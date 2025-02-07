#include <windows.h>
#include <stdio.h>
#include <string.h>

const char* BLOCKED_SITES[] = { "YouTube", "youtube" };
const int BLOCKED_COUNT = sizeof(BLOCKED_SITES) / sizeof(BLOCKED_SITES[0]);

// 창 제목에 특정 차단 사이트가 포함되어 있는지 확인
BOOL IsBlockedSite(const char* title) {
    for (int i = 0; i < BLOCKED_COUNT; i++) {
        if (strstr(title, BLOCKED_SITES[i]) != NULL) {
            return TRUE;
        }
    }
    return FALSE;
}

// 현재 브라우저 창의 제목을 확인하고 출력 및 차단
void MonitorBrowserAndBlock() {
    HWND hWnd = GetForegroundWindow();  // 현재 활성 창 핸들 가져오기
    char title[256];

    if (hWnd && GetWindowTextA(hWnd, title, sizeof(title))) {
        printf("현재 접속한 사이트: %s\n", title);

        if (IsBlockedSite(title)) {
            printf("[차단] %s 창을 닫습니다.\n", title);
            PostMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }
    else {
        printf("현재 브라우저 창을 찾을 수 없습니다.\n");
    }
}

int main() {
    printf("사이트 모니터링 및 유튜브 차단 프로그램 실행 중...\n");

    while (1) {
        MonitorBrowserAndBlock();
        Sleep(3000);  // 3초마다 검사
    }

    return 0;
}


/*
사이트 모니터링 및 유튜브 차단 프로그램 실행 중...
현재 접속한 사이트: D:\swgtest.exe
현재 브라우저 창을 찾을 수 없습니다.
현재 접속한 사이트: 새 탭 - Whale
현재 접속한 사이트: NAVER - Whale
현재 접속한 사이트: NAVER - Whale
현재 접속한 사이트: 제목 없음 - Whale
현재 접속한 사이트: 네이버 날씨 홈 - Whale
현재 접속한 사이트: NAVER - Whale
현재 접속한 사이트: D:\swgtest.exe
현재 접속한 사이트: 새 탭 - Chrome
현재 접속한 사이트: 새 탭 - Chrome
현재 접속한 사이트: YouTube - Chrome
[차단] YouTube - Chrome 창을 닫습니다.
현재 접속한 사이트: 사이트 차단 프로그램 C - Chrome
현재 접속한 사이트: 새 탭 - Chrome
현재 접속한 사이트: YouTube - Chrome
[차단] YouTube - Chrome 창을 닫습니다.

....

*/
