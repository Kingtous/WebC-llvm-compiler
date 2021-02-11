// this is a comment。这是个注释xxx
int param = 100;
int nums[10][10][10][10];
// 求[1,param)整数和
int getNum(int param){
    int cnt = 0;
    for(int index=0; index<param; index=index+1){
        if (index % 2 == 0){
            if (index % 3 == 0){
                cnt = cnt + index;
                while (cnt > 0) {
                    cnt = cnt - 1;
                    while (cnt < 0) {
                        return 0;
                    }
                    break;
                }
            }
        }
        continue;
    }
    return cnt;
}
