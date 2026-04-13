#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "Job.hpp"

// Forward declarations of algorithm functions (adapted from their source files)
struct Result {
    std::vector<Job> bestOrder;
    int totalPenalty = 0;
};

// DP Implementation
struct DPResult {
    int totalPenalty;
    std::vector<int> onTimeJobs;
    std::vector<int> lateJobs;
};

DPResult runDP(std::vector<Job> jobs) {
    if (jobs.empty()) return {0, {}, {}};
    std::sort(jobs.begin(), jobs.end());
    const size_t size = jobs.size();
    int total_time = 0;
    for (const auto& j : jobs) total_time += j.time;
    std::vector dp(size + 1, std::vector<int>(total_time + 1, 0));
    for (int i = 1; i <= static_cast<int>(size); ++i) {
        const int p_i = jobs[i - 1].penalty;
        const int t_i = jobs[i - 1].time;
        const int d_i = jobs[i - 1].deadline;
        for (int t = 0; t <= total_time; ++t) {
            int skipPenalty = p_i + dp[i - 1][t];
            int doPenalty = 2000000000;
            if (t >= t_i && t <= d_i) doPenalty = dp[i - 1][t - t_i];
            dp[i][t] = std::min(doPenalty, skipPenalty);
            if (t > 0) dp[i][t] = std::min(dp[i][t], dp[i][t - 1]);
        }
    }
    int totalPenalty = dp[size][total_time];
    std::vector<int> onTimeJobs, lateJobs;
    int curTime = total_time;
    for (int t = 0; t <= total_time; ++t) {
        if (dp[size][t] == totalPenalty) {
            curTime = t;
            break;
        }
    }
    for (int i = static_cast<int>(size); i > 0; i--) {
        const int jobTime = jobs[i - 1].time;
        const int jobDeadline = jobs[i - 1].deadline;
        while (curTime > 0 && dp[i][curTime] == dp[i][curTime - 1]) curTime--;
        if (curTime >= jobTime && curTime <= jobDeadline && dp[i][curTime] == dp[i - 1][curTime - jobTime]) {
            onTimeJobs.push_back(jobs[i - 1].id);
            curTime -= jobTime;
        } else {
            lateJobs.push_back(jobs[i - 1].id);
        }
    }
    std::reverse(onTimeJobs.begin(), onTimeJobs.end());
    return {totalPenalty, onTimeJobs, lateJobs};
}

// BF Implementation
int calculatePenalty(const std::vector<Job>& jobs) {
    int time = 0, penalty = 0;
    for (const auto& j : jobs) {
        time += j.time;
        if (time > j.deadline) penalty += j.penalty;
    }
    return penalty;
}

void BFHelper(const std::vector<Job>& jobs, std::vector<Job>& current, std::vector<bool>& used, Result& best) {
    if (current.size() == jobs.size()) {
        int p = calculatePenalty(current);
        if (p < best.totalPenalty) {
            best.totalPenalty = p;
            best.bestOrder = current;
        }
        return;
    }
    for (size_t i = 0; i < jobs.size(); i++) {
        if (used[i]) continue;
        used[i] = true;
        current.push_back(jobs[i]);
        BFHelper(jobs, current, used, best);
        current.pop_back();
        used[i] = false;
    }
}

Result runBF(const std::vector<Job>& jobs) {
    Result best;
    best.totalPenalty = 2147483647;
    std::vector<Job> current;
    std::vector<bool> used(jobs.size(), false);
    BFHelper(jobs, current, used, best);
    return best;
}

// D&C Implementation
struct DCState {
    int bestSavedPenalty = -1;
    std::vector<char> best_on_time;
};

void DCHelper(const std::vector<Job>& jobs, int i, int currentTime, int savedPenalty, std::vector<char>& on_time_flags, int remainingPenaltySum, DCState& state) {
    if (savedPenalty + remainingPenaltySum <= state.bestSavedPenalty) return;
    if (i == static_cast<int>(jobs.size())) {
        if (savedPenalty > state.bestSavedPenalty) {
            state.bestSavedPenalty = savedPenalty;
            state.best_on_time = on_time_flags;
        }
        return;
    }
    int p_sum = remainingPenaltySum - jobs[i].penalty;
    on_time_flags[i] = 0;
    DCHelper(jobs, i + 1, currentTime, savedPenalty, on_time_flags, p_sum, state);
    if (currentTime + jobs[i].time <= jobs[i].deadline) {
        on_time_flags[i] = 1;
        DCHelper(jobs, i + 1, currentTime + jobs[i].time, savedPenalty + jobs[i].penalty, on_time_flags, p_sum, state);
        on_time_flags[i] = 0;
    }
}

Result runDC(std::vector<Job> jobs) {
    std::sort(jobs.begin(), jobs.end());
    int totalPenaltyAll = 0;
    for (const auto& j : jobs) totalPenaltyAll += j.penalty;
    DCState state;
    state.best_on_time.assign(jobs.size(), 0);
    std::vector<char> currentTake(jobs.size(), 0);
    DCHelper(jobs, 0, 0, 0, currentTake, totalPenaltyAll, state);
    Result res;
    std::vector<Job> onTime, late;
    for (size_t i = 0; i < jobs.size(); ++i) {
        if (state.best_on_time[i]) onTime.push_back(jobs[i]);
        else late.push_back(jobs[i]);
    }
    res.bestOrder = onTime;
    res.bestOrder.insert(res.bestOrder.end(), late.begin(), late.end());
    res.totalPenalty = totalPenaltyAll - state.bestSavedPenalty;
    return res;
}

// GUI Constants and Globals
#define ID_BTN_ADD 1001
#define ID_BTN_RUN 1002
#define ID_BTN_CLEAR 1003
#define ID_EDIT_ID 2001
#define ID_EDIT_TIME 2002
#define ID_EDIT_DEADLINE 2003
#define ID_EDIT_PENALTY 2004
#define ID_LIST_JOBS 3001
#define ID_TEXT_RESULTS 3002

std::vector<Job> g_jobs;
HWND hEditID, hEditTime, hEditDeadline, hEditPenalty, hListJobs, hTextResults;

void UpdateJobList() {
    std::wstringstream ss;
    for (const auto& j : g_jobs) {
        ss << L"Job " << j.id << L": Time=" << j.time << L", Deadline=" << j.deadline << L", Penalty=" << j.penalty << L"\r\n";
    }
    SetWindowTextW(hListJobs, ss.str().c_str());
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CreateWindowW(L"Static", L"ID:", WS_VISIBLE | WS_CHILD, 10, 10, 30, 20, hwnd, nullptr, nullptr, nullptr);
            hEditID = CreateWindowW(L"Edit", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 45, 10, 50, 20, hwnd, (HMENU)ID_EDIT_ID, nullptr, nullptr);
            
            CreateWindowW(L"Static", L"Time:", WS_VISIBLE | WS_CHILD, 105, 10, 40, 20, hwnd, nullptr, nullptr, nullptr);
            hEditTime = CreateWindowW(L"Edit", L"5", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 150, 10, 50, 20, hwnd, (HMENU)ID_EDIT_TIME, nullptr, nullptr);
            
            CreateWindowW(L"Static", L"Deadline:", WS_VISIBLE | WS_CHILD, 210, 10, 60, 20, hwnd, nullptr, nullptr, nullptr);
            hEditDeadline = CreateWindowW(L"Edit", L"10", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 275, 10, 50, 20, hwnd, (HMENU)ID_EDIT_DEADLINE, nullptr, nullptr);
            
            CreateWindowW(L"Static", L"Penalty:", WS_VISIBLE | WS_CHILD, 335, 10, 55, 20, hwnd, nullptr, nullptr, nullptr);
            hEditPenalty = CreateWindowW(L"Edit", L"20", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 395, 10, 50, 20, hwnd, (HMENU)ID_EDIT_PENALTY, nullptr, nullptr);
            
            CreateWindowW(L"Button", L"Add Job", WS_VISIBLE | WS_CHILD, 455, 10, 80, 20, hwnd, (HMENU)ID_BTN_ADD, nullptr, nullptr);
            CreateWindowW(L"Button", L"Clear", WS_VISIBLE | WS_CHILD, 545, 10, 80, 20, hwnd, (HMENU)ID_BTN_CLEAR, nullptr, nullptr);
            
            hListJobs = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 10, 40, 615, 100, hwnd, (HMENU)ID_LIST_JOBS, nullptr, nullptr);
            
            CreateWindowW(L"Button", L"Run All Algorithms", WS_VISIBLE | WS_CHILD, 10, 150, 615, 30, hwnd, (HMENU)ID_BTN_RUN, nullptr, nullptr);
            
            hTextResults = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 10, 190, 615, 250, hwnd, (HMENU)ID_TEXT_RESULTS, nullptr, nullptr);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_BTN_ADD) {
                WCHAR buf[32];
                Job j{};
                GetWindowTextW(hEditID, buf, 32); j.id = _wtoi(buf);
                GetWindowTextW(hEditTime, buf, 32); j.time = _wtoi(buf);
                GetWindowTextW(hEditDeadline, buf, 32); j.deadline = _wtoi(buf);
                GetWindowTextW(hEditPenalty, buf, 32); j.penalty = _wtoi(buf);
                g_jobs.push_back(j);
                UpdateJobList();
                // Auto increment ID for convenience
                swprintf(buf, 32, L"%d", j.id + 1);
                SetWindowTextW(hEditID, buf);
            } else if (LOWORD(wParam) == ID_BTN_CLEAR) {
                g_jobs.clear();
                UpdateJobList();
                SetWindowTextW(hTextResults, L"");
                SetWindowTextW(hEditID, L"1");
            } else if (LOWORD(wParam) == ID_BTN_RUN) {
                if (g_jobs.empty()) {
                    MessageBoxW(hwnd, L"Please add some jobs first!", L"Error", MB_ICONERROR);
                    break;
                }
                std::wstringstream res;
                
                // DP
                DPResult dp = runDP(g_jobs);
                res << L"--- Dynamic Programming ---\r\nTotal Penalty: " << dp.totalPenalty << L"\r\nOrder: ";
                for (int id : dp.onTimeJobs) res << id << L" ";
                for (int id : dp.lateJobs) res << id << L" ";
                res << L"\r\n\r\n";
                
                // BF
                Result bf = runBF(g_jobs);
                res << L"--- Brute Force ---\r\nTotal Penalty: " << bf.totalPenalty << L"\r\nOrder: ";
                for (const auto& j : bf.bestOrder) res << j.id << L" ";
                res << L"\r\n\r\n";
                
                // D&C
                Result dc = runDC(g_jobs);
                res << L"--- Divide & Conquer ---\r\nTotal Penalty: " << dc.totalPenalty << L"\r\nOrder: ";
                for (const auto& j : dc.bestOrder) res << j.id << L" ";
                res << L"\r\n";
                
                SetWindowTextW(hTextResults, res.str().c_str());
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const WCHAR CLASS_NAME[] = L"JobSchedulingGUIV1";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"Job Scheduling Algorithms - GUI", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 650, 500, nullptr, nullptr, hInstance, nullptr);
    if (hwnd == nullptr) return 0;
    ShowWindow(hwnd, nCmdShow);
    
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
