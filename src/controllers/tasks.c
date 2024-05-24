#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../config/database.c"

#define MAX_ROWS 100

typedef struct {
    int id;
    char title[100];
    char description[1000];
    int status;
    char created_at[30];
    char updated_at[30];
} Row;


Row rows[MAX_ROWS];

char *create_json_string(char *json_str, int num_rows) {
    if (json_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    char *ptr = json_str;
    ptr += sprintf(ptr, "[\n");
    for (int i = 0; i < num_rows; i++) {
        ptr += sprintf(ptr, "  {\n");
        ptr += sprintf(ptr, "    \"id\": %d,\n", rows[i].id);
        ptr += sprintf(ptr, "    \"title\": \"%s\",\n", rows[i].title);
        ptr += sprintf(ptr, "    \"description\": \"%s\",\n", rows[i].description);
        ptr += sprintf(ptr, "    \"status\": %d,\n", rows[i].status);
        ptr += sprintf(ptr, "    \"created_at\": \"%s\",\n", rows[i].created_at);
        ptr += sprintf(ptr, "    \"updated_at\": \"%s\"\n", rows[i].updated_at);
        ptr += sprintf(ptr, "  }%s\n", i < num_rows - 1 ? "," : "");
    }
    ptr += sprintf(ptr, "]\n");

    return json_str;
}

void fetch_all(char *json_str) {
    int num_rows = 0;

    MYSQL *con = connect_to_db();
    if (mysql_query(con, "SELECT * FROM tasks")) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        if (num_rows >= MAX_ROWS) {
            fprintf(stderr, "Maximum number of rows exceeded\n");
            break;
        }
        rows[num_rows].id = atoi(row[0]);
        strncpy(rows[num_rows].title, row[1] ? row[1] : "", sizeof(rows[num_rows].title));
        strncpy(rows[num_rows].description, row[2] ? row[2] : "", sizeof(rows[num_rows].description));
        rows[num_rows].status = atoi(row[3]);
        strncpy(rows[num_rows].created_at, row[4] ? row[4] : "", sizeof(rows[num_rows].created_at));
        strncpy(rows[num_rows].updated_at, row[5] ? row[5] : "", sizeof(rows[num_rows].updated_at));
        num_rows++;
    }

    mysql_free_result(result);
    mysql_close(con);

    create_json_string(json_str, num_rows);
}