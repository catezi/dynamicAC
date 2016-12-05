#include "Headers/dynamicAC.h"
#include "Headers/define.h"
#include "Headers/transform.h"
#include "Headers/readfile.h"
#include "Headers/dynamictool.h"
#include "Headers/matchthread.h"



void *matchthread(void* arg) {
    int max = 1000;
    int i = 0;
    while (true) {
        string message = getmessage("message.txt");
        int successnum[patternnum] = {0};
        if (rootnum == 0) {
            cout << "the matching thread is using root to match." << endl;
            threadrootnum = 0;
            searchAC(root, (char*)message.c_str(), message.size(), successnum);
        }
        else {
            cout << "the matching thread is using root1 to match." << endl;
            threadrootnum = 1;
            searchAC(root1, (char*)message.c_str(), message.size(), successnum);
        }
        if (i ++ >= max) {
            finish = true;
            break;
        }
    }
    cout << "The match thread has been finished." << endl;
    return arg;
}
