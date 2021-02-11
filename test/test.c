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
                if (cnt > 0) {
                    break;
                    cnt = cnt + 1;
                } else {
                    cnt = cnt - 1;
                }
            }
        }
        continue;
    }
    return cnt;
}
