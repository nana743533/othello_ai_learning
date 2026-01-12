#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  // 引数チェック: ドメイン名が渡されているか確認
  if (argc != 2) {
    fprintf(stderr, "使用方法: %s <hostname>\n", argv[0]);
    return 1;
  }

  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  // hints構造体の初期化
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // AF_INET (IPv4) でも AF_INET6 (IPv6) でも可
  hints.ai_socktype = SOCK_STREAM; // TCPストリーム

  // DNS解決を実行
  // argv[1]: ドメイン名 (例: google.com)
  // NULL: サービス名/ポート番号 (今回はIPのみ欲しいので不要)
  // &hints: 検索条件
  // &res: 結果の格納先（リンクリスト）
  if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfoエラー: %s\n", gai_strerror(status));
    return 2;
  }

  printf("%s のIPアドレス:\n\n", argv[1]);

  // 結果のリンクリストを走査
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;

    // IPv4かIPv6かでポインタのキャスト先を変える
    if (p->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // IPアドレスを文字列に変換 (バイナリ -> テキスト)
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }

  // メモリ解放
  freeaddrinfo(res);

  return 0;
}
