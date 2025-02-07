#include <windows.h>
#include <tlhelp32.h>
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

// 특정 프로세스를 종료하는 함수
void KillProcessByName(const char* processName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, processName) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    printf("[차단] %s 프로세스를 종료했습니다.\n", processName);
                }
            }
        } while (Process32Next(hSnap, &pe));
    }

    CloseHandle(hSnap);
}

// 현재 브라우저 창의 제목을 확인하고 차단
void MonitorBrowserAndBlock() {
    HWND hWnd = GetForegroundWindow();  // 현재 활성 창 핸들 가져오기
    char title[256];

    if (hWnd && GetWindowTextA(hWnd, title, sizeof(title))) {
        printf("현재 접속한 사이트: %s\n", title);

        if (IsBlockedSite(title)) {
            printf("[차단] %s 창을 닫습니다.\n", title);
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            Sleep(500);  // 프로세스 종료 전에 약간의 대기 시간 추가
            KillProcessByName("chrome.exe");  // 크롬 브라우저 강제 종료
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
