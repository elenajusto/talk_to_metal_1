int global_variable = 10;         // Should start off as 10
int uninitialised_global_variable  // Should start off as zero

int main(void) { 
    int local_variable = 20;                        // Should be 20 
    
    static int static_local_variable = 30;          // Should be 30

    static int uninitialised_static_local_variable; // Should be zero

    static int sum;                                 // Should be zero
    
    sum =   global_variable + uninitialised_global_variable +
            local_variable + static_local_variable +
            uninitialised_static_local_variable;
    
    if sum(==60){
        while(1) {}
    } else {
        return 0;
    }
}