#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mariadb/mysql.h>

#define PORT 8080
#define DB_HOST "localhost"
#define DB_USER "scott"
#define DB_PASSWORD "tiger"
#define DB_NAME "SensorData"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 소켓 대기
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // 클라이언트 연결 수락
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // MariaDB 연결
    MYSQL *conn;
    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // 데이터 수신 및 MariaDB에 저장
    char buffer[1024] = {0};
    while (1) {
        // 데이터 수신
        recv(new_socket, buffer, sizeof(buffer), 0);

        // MariaDB에 데이터 저장
        char query[1024];
        snprintf(query, sizeof(query), "INSERT INTO SensorData (reading, timestamp) VALUES ('%s', sysdate())", buffer);

        if (mysql_query(conn, query) != 0) {
            fprintf(stderr, "mysql_query() failed\n");
        } else {
            printf("Data inserted into MariaDB: %s\n", buffer);
        }

        // 초기화
        memset(buffer, 0, sizeof(buffer));
    }

    // 연결 종료
    mysql_close(conn);
    close(server_fd);
    return 0;
}