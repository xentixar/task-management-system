#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

MYSQL *connect_to_db() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "127.0.0.1", "root", "", "task_management_system", 0, NULL, 0) == NULL) {
        finish_with_error(con);
    }

    return con;
}