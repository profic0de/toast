#include "kit.h"

//

void error_message(const char* filename, size_t line, size_t column, char* error) {
    fprintf(stderr, "%s:%zu:%zu: \033[1;31merror:\033[0m %s\n", filename, line, column, error);
    
    FILE* fd = fopen(filename, "r");
    if (!fd) return;
    
    char line_buffer[1024] = {0};
    size_t current_line = 0;
    
    while (fgets(line_buffer, sizeof(line_buffer), fd)) {
        if (current_line == line) {
            // Find word boundaries around the error column
            size_t col_idx = column - 1;
            size_t word_start = col_idx;
            size_t word_end = col_idx;
            
            // Find start of word
            while (word_start > 0 && (isalnum(line_buffer[word_start-1]) || line_buffer[word_start-1] == '_')) {
                word_start--;
            }
            
            // Find end of word
            while (isalnum(line_buffer[word_end]) || line_buffer[word_end] == '_') {
                word_end++;
            }
            
            // Print line with colored token
            fprintf(stderr, "%5zu | ", line);
            for (size_t i = 0; line_buffer[i] && line_buffer[i] != '\n'; i++) {
                if (i >= word_start && i < word_end) {
                    fprintf(stderr, "\033[1;31m%c\033[0m", line_buffer[i]);
                } else {
                    fprintf(stderr, "%c", line_buffer[i]);
                }
            }
            fprintf(stderr, "\n");
            
            // Print underline
            fprintf(stderr, "      | ");
            for (size_t i = 0; i < word_start; i++) fprintf(stderr, " ");
            
            fprintf(stderr, "\033[1;31m");
            fprintf(stderr, "^");
            for (size_t i = word_start + 1; i < word_end; i++) {
                fprintf(stderr, "~");
            }
            fprintf(stderr, "\033[0m\n");
            break;
        }
        current_line++;
    }
    
    fclose(fd);
}