int global_variable = 10;
int uninit_global_variable; // Should be 0

int main(void) {
    int local_variable = 20;
    static int static_local_variable = 30;
    static int uninit_static_local_variable; // Should be 0
    static int sum; // Should be 0
    
    sum = global_variable + uninit_global_variable +
          local_variable + static_local_variable +
          uninit_static_local_variable;
           
    if (sum == 60){
        while (1) { }
    } else {
        return 0;
    }
}