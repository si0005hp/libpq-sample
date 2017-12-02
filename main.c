#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

typedef struct Project_t {
  int id;
  char *name;
} Project;

void do_exit(PGconn *conn);
Project* deserialize_row(PGresult *res, int row_idx);

int main(int argc, char **argv) {
  PGconn *conn = PQconnectdb(
      "host=10.0.2.2 dbname=postgres user=postgres password=postgres");
  if (PQstatus(conn) == CONNECTION_BAD) {
    fprintf(stderr, "Failed to connect: %s\n", PQerrorMessage(conn));
    do_exit(conn);
  }

  PGresult *res = PQexec(conn, "SELECT * FROM project");

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("No data retrieved\n");
    PQclear(res);
    do_exit(conn);
  }

  int rows = PQntuples(res);
  for (int i = 0; i < rows; i++) {
    Project *p = deserialize_row(res, i);
    printf("%d %s\n", p->id, p->name);
    free(p);
  }

  PQclear(res);
  PQfinish(conn);

  return 0;
}

Project* deserialize_row(PGresult *res, int row_idx) {
  Project *p = malloc(sizeof(Project));
  p->id = atoi(PQgetvalue(res, row_idx, 0));
  p->name = PQgetvalue(res, row_idx, 1);
  return p;
}

void do_exit(PGconn *conn) {
  PQfinish(conn);
  exit(1);
}

