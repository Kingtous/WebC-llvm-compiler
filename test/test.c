str f(int a){
    ret 'hello';
}
void call_func(func_ptr<str,int> f){
    str resp = f(5);
    echo('resp is ',resp);
}
int main(){
    call_func(f);
    ret 0;
}