int global[5] = {1, 2, 3, 4, 5};

int ctotal = 0;

int recursive_call(int cnt) {
    if (cnt == 0) {
        ret ctotal;
    }
//    wh(cnt > 0){
//        cnt = cnt -1;
//    }
    ctotal = ctotal + cnt;
    ret recursive_call(cnt - 1);
}

int sum(int cnt){
    ret ((1+cnt)*cnt)/2;
}

int echo(){
    lp(int i =0;i<10;i=i+1){
        ret 1;
    }
    ret 0;
}
