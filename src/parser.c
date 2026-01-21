#include "../include/parser.h"



// Helper: parse "a/b" or "a"
int parse_fraction(const char *token, Fraction *frac) {
    int num, denom;
    
    if (sscanf(token, "%d/%d", &num, &denom) == 2) {
        createFraction(frac, num, denom);
        return 1;
    } else if (sscanf(token, "%d", &num) == 1) {
        createFraction(frac, num, 1);
        return 1;
    }
    return 0;
}

// Parse sign string
enum EquationSign parse_sign(const char *sign) {
    if (strcmp(sign, "<=") == 0) return LOWER_OR_EQUAL;
    if (strcmp(sign, "=") == 0) return EQUAL;
    if (strcmp(sign, ">=") == 0) return HIGHER_OR_EQUAL;
    return LOWER_OR_EQUAL; // default
}

// Main parser
int parse_lp_file(const char *filename, Constraints *cons, ObjectiveFunction *obj,enum LinearProblemType *obj_type) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE];
    int cons_count = 0, var_count = 0, cons_read = 0;
    int reading_coeffs = 0, reading_obj = 0;
    *obj_type = MAX; // default

    while (fgets(line, MAX_LINE, fp)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        // Skip empty lines and comments
        if (line[0] == '#' || line[0] == '\0') continue;

        // Parse header lines
        if (strstr(line, "OBJECTIVE:") == line) {
            if (strstr(line, "MAX")) *obj_type = MAX;
            else if (strstr(line, "MIN")) *obj_type = MIN;
            continue;
        }
        if (strstr(line, "VARIABLES:") == line) {
            sscanf(line, "VARIABLES: %d", &var_count);
            cons->n = var_count;
            for (int i = 0; i < var_count; i++) cons->variables_type[i] = NORMAL;
            continue;
        }
        if (strstr(line, "CONSTRAINTS:") == line) {
            sscanf(line, "CONSTRAINTS: %d", &cons_count);
            cons->m = cons_count;
            continue;
        }
        if (strstr(line, "COEFFS:") == line) {
            reading_coeffs = 1;
            reading_obj = 0;
            continue;
        }
        if (strstr(line, "OBJ:") == line) {
            reading_coeffs = 0;
            reading_obj = 1;
            continue;
        }

        // Parse constraint line
        if (reading_coeffs && cons_read < cons_count) {
            char *token;
            char copy[MAX_LINE];
            strcpy(copy, line);
            token = strtok(copy, " \t");
            int col = 0;
            while (token && col < var_count) {
                if (!parse_fraction(token, &cons->constraints_coeff[cons_read][col])) {
                    fprintf(stderr, "Coefficient parse error: %s\n", token);
                    fclose(fp);
                    return 0;
                }
                col++;
                token = strtok(NULL, " \t");
            }
            // Next token should be the sign
            if (token) {
                cons->constraints_sign[cons_read] = parse_sign(token);
                token = strtok(NULL, " \t");
                // Next token is RHS
                if (token) {
                    if (!parse_fraction(token, &cons->RHS[cons_read])) {
                        fprintf(stderr, "RHS parse error: %s\n", token);
                        fclose(fp);
                        return 0;
                    }
                }
            }
            cons_read++;
        }

        // Parse objective line
        if (reading_obj) {
            char *token;
            char copy[MAX_LINE];
            strcpy(copy, line);
            token = strtok(copy, " \t");
            int col = 0;
            while (token && col < var_count) {
                if (!parse_fraction(token, &obj->coefficiants[col])) {
                    fprintf(stderr, "Objective coeff parse error: %s\n", token);
                    fclose(fp);
                    return 0;
                }
                col++;
                token = strtok(NULL, " \t");
            }
            obj->length = var_count;
        }
    }

    fclose(fp);
    return 1;
}



