#include "Headers/dynamicAC.h"
#include "Headers/define.h"
#include "Headers/transform.h"
#include "Headers/readfile.h"
#include "Headers/dynamictool.h"
#include "Headers/matchthread.h"

Tree root;
Tree root1;
char pattern[patternnum][patternlength] = {{'\0'}};
int patternnumber = 0;
int pattnum = 0;
long number = 15;
bool finish = false;
int rootnum = 0;
int threadrootnum = 0;
pthread_t id;
pthread_mutex_t mutex;



long getCurrentTime() {
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void initpattern() {
    ifstream in("2-1-2-50000-0x.txt");
    string s;
    while (!in.eof()) {
        string ss;
        getline(in, ss);
        s += ss;
    }
    getpatterns(pattern, "2-1-2-1000-0x.txt");
    pattnum = patternnumber;
    root=buildingTree();
    buildingFailPath(root);
}

void addpatternsfromfile(Tree temproot, int num, string filename) {
    cout << "add to root" << num << endl;
    char addpatterns[patternnum][patternlength] = {{'\0'}};
    getpatterns(addpatterns, filename);
    for (int i = 0; addpatterns[i][0] != '\0'; i ++) {
        string ss = addpatterns[i];
        addpattern(temproot, ss, number ++);
    }
    buildingFailPath(temproot);
    rootnum = num;
}

void deletepatternsfromfile(Tree temproot, int num, string filename) {
    cout << "delete from root!" << num << endl;
    char deletepatterns[patternnum][patternlength] = {{'\0'}};
    getpatterns(deletepatterns, filename);
    for (int i = 0; deletepatterns[i][0] != '\0'; i ++) {
        string ss = deletepatterns[i];
        deletepattern(temproot, ss);
    }
    buildingFailPath(temproot);
    rootnum = num;
}

int main()
{
    pthread_mutex_init (&mutex,NULL);
    int res = pthread_create(&id, NULL, matchthread, NULL);
    if (res) {
        cout << "create thread failed!" << endl;
    }

    while (true) {
        char order1[10] = {'\0'};
        char order2[100] = {'\0'};
        scanf("%s %s", order1, order2);
        string s1 = order1;
        string s2 = order2;
        if (s1 == "-a") {
            if (rootnum == 0) {
                addpatternsfromfile(root1, 1, s2);
                while (threadrootnum != 1) {
                    cout << "please wait to the other root to update!" << endl;
                    sleep(1);
                }
                addpatternsfromfile(root, 1, s2);
            }
            else {
                addpatternsfromfile(root, 0, s2);
                while (threadrootnum != 0) {
                    cout << "please wait to the other root to update!" << endl;
                    sleep(1);
                }
                addpatternsfromfile(root1, 0, s2);
            }
        }
        else if (s1 == "-d") {
            if (rootnum == 0) {
                deletepatternsfromfile(root1, 1, s2);
                while (threadrootnum != 1) {
                    cout << "please wait to the other root to update!" << endl;
                    sleep(1);
                }
                deletepatternsfromfile(root, 1, s2);
            }
            else {
                deletepatternsfromfile(root, 0, s2);
                while (threadrootnum != 0) {
                    cout << "please wait to the other root to update!" << endl;
                    sleep(1);
                }
                deletepatternsfromfile(root1, 0, s2);
            }
        }
        if (finish) {
            break;
        }
    }
    long t1 = GetTickCount();
    long t2 = GetTickCount();
    cout << t2-t1 << endl;

    return 0;
}
