#include <stdlib.h>
#include <stdio.h>

// Declare dynamic arrays/vectors and global variables
int *resource = NULL;
int *available = NULL;
int *max_claim = NULL;
int *allocated = NULL;
int *need = NULL;

int num_processes;
int num_resources;

//*************************************************************
void print_vector(int *vector, char *title) {
    // declare local variables
    int j;
    printf("\n%s\n", title);

    // for loop j: print each resource index
    for (j = 0; j < num_resources; j++) {
        printf("\tr%d", j);
    }

    printf("\n");

    // for loop j: print value of vector[j]
    for(j = 0; j < num_resources; j++) {
        printf("\t%d", vector[j]);
    }
}

//*************************************************************
void print_array(int *matrix, char *title) {
    // declare local variables
    int i, j;
    printf("\n%s\n", title);

    // for loop j: print each resource index
    for(j =0; j < num_resources; j++) {
        printf("\tr%d", j);
    }

    printf("\n");

    // for each process i
    for(i = 0; i < num_processes; i++){
        printf("p%d", i);
        // for loop j: print value of vector[j]
        for(j = 0; j < num_resources; j++) {
            printf("\t%d", matrix[i * num_resources + j]);
        }
        printf("\n");
    }
}

//**************************************************************
void enter_claim_graph() {
    // prompt for number of resources: num_resources
    printf("Enter number of resources:");
    scanf("%d", &num_resources);

    // allocate memory for vectors
    resource = (int *)malloc(num_resources * sizeof(int));
    available = (int *)malloc(num_resources * sizeof(int));

    // for each resource, prompt for number of units, set resource and available vectors indices
    printf("Enter number of units for resources (r0 to r%d):", num_resources - 1);

    for (int j = 0; j < num_resources; j++){
        scanf("%d", &resource[j]);
        available[j] = resource[j];
    }

    // prompt for number of processes: num_processes
    printf("Enter number of processes:");
    scanf("%d", &num_processes);

    // allocate memory for arrays
    max_claim = (int *)malloc(num_processes * num_resources * sizeof(int));
    allocated = (int *)malloc(num_processes * num_resources * sizeof(int));
    need = (int *)malloc(num_processes * num_resources * sizeof(int));

    // for each process, for each resource, prompt for maximum number of units requested by process, update maxclaim and need arrays
    for(int i = 0; i < num_processes; i++){
        printf("Enter maximum number of units process p%d will claim from each resource (r0 to r%d): ", i, num_resources-1);
        for (int j = 0; j < num_resources; j++){
            scanf("%d", &max_claim[i * num_resources + j]);
            need[i * num_resources + j] = max_claim[i * num_resources + j];
        }//for resource j
    }//for process i

    // for each process, for each resource, prompt for number of resource units allocated to process
    for(int i = 0; i < num_processes; i++){
        printf("Enter number of units of each resource (r0 to r%d) currently allocated to process p%d: ", num_resources-1, i);
        for (int j = 0; j < num_resources; j++){
            scanf("%d", &allocated[i*num_resources+j]);
            need[i*num_resources+j] -= allocated[i*num_resources+j];
            available[j] -= allocated[i*num_resources+j];
        }//for resource j
    }//for process i
    // print resource vector, available vector, maxclaim array, allocated array, need array
    print_vector(resource, "Resources:");
    print_vector(available, "Available:");

    print_array(max_claim, "Max Claim:");
    print_array(allocated, "Allocated:");
    print_array(need, "Need:");
}

//**************************************************************
void request_resource() {
    // declare local variables
    int i, j, n;

    // prompt for process i, resource j, and number of units n requested
    printf("Enter requesting process: p");
    scanf("%d", &i);

    printf("Enter requested resource: r");
    scanf("%d", &j);

    printf("Enter number of units p%d is requesting from resource r%d:", i,j);
    scanf("%d", &n);

    // if enough units allocated
    if ((n <= available[j]) && (n <= need[i*num_resources+j])) {
        // reduce number of available units
        available[j] -= n;
        // increase number of allocated units
        allocated[i * num_resources + j] += n;
        // reduce number of needed units
        need[i * num_resources + j] -= n;

        // print updated available, allocated, and need vectors/matrices
        print_vector(available, "Available");
        print_array(allocated, "Allocated");
        print_array(need, "Need");
    }
        // else
    else{
        //print message that release cannot be performed
        printf("Request could not be performed");
    }
}

//******************************************************************
void release_resource() {
    // declare local variables
    int i,j,n;

    // prompt for process i, resource j, and number of units n requested
    printf("Enter releasing processor: p");
    scanf("%d", &i);

    printf("Enter released resource r:");
    scanf("%d", &j);

    printf("Enter number of units process p%d is releasing from resource r%d:", i,j);
    scanf("%d", &n);

    // if enough units allocated
    if ((n <= available[j]) && (n <= need[i*num_resources+j])){
        // increase number of available units
        available[j] += n;
        // reduce number of allocated units
        allocated[i * num_resources + j] -= n;
        // increase number of needed units
        need[i * num_resources + j] += n;

        // print updated available, allocated, and need vectors/matrices
        print_vector(available, "Available:");
        print_array(allocated, "Allocated:");
        print_array(need, "Need:");
    }
        // else
    else{
        //print message that release cannot be performed
        printf("Release was unable to be performed");
    }
}

//******************************************************************
void safe_sequence(int * matrix, int * vector) {
    // declare local variables
    int num_done = 0;
    int i,j;
    int *done = (int *)calloc(num_processes , sizeof(int));
    int less_than_or_equal = 1;
    int at_least_one = 1;
    int *sequence = (int *)calloc(num_processes , sizeof(int));

    // while not all processes are processed
    while((num_done < num_processes) && (at_least_one == 1)){
        at_least_one = 0;
        // for each process
        for( i = 0; i < num_processes; i++){
            less_than_or_equal = 1;
            // if process has not been processed yet
            if(done[i] == 0){
                // print message comparing need vector with available vector
                printf("Compairing: < ");

                // for each resource j:
                for( j = 0; j < num_resources; j++){
                    printf("%d ", matrix[i * num_resources + j]);
                }

                printf("> <= <");
                for(j = 0; j < num_resources; j++) {
                    printf("%d ", vector[j]);
                }

                printf("> : ");
                for(j = 0; j < num_resources; j++) {
                    //check for safe processing by comparing process' need vector to available vector
                    less_than_or_equal &= (need[i * num_resources + j] <= available[j]);
                }

                // if each resource is available
                if(less_than_or_equal == 1){
                    // print message that process can be processed
                    printf("Process p%d can be processed\n", i);
                    sequence[num_done] = i;
                    // update number of available units of resource
                    for(j = 0; j < num_resources; j++){
                        //update number of available units of resource
                        available[j] += allocated[i*num_resources+j];
                        //free all resources allocated to process
                        allocated[j] = 0;
                    }
                    // increment number of sequenced processes, set done vector at index i to 1
                    num_done++;
                    done[i] = 1;
                    at_least_one = 1;
                }
                    // else print message that process cannot be processed
                else{
                    printf("Process p%d could not be processed\n", i);
                }
            }//if done
        }//for i
    }//while
    //if (no process was processed in the final round of the for-loop)
    if(at_least_one == 0){
        // print message of deadlock among processes not processed
        printf("Deadlock among processes\n");
    }
        // else print safe sequence of processes
    else{
        printf("Safe sequence of processes: ");
        for(i = 0; i < num_processes; i++) {
            printf("p%d ", sequence[i]);
        }
        printf("\n");
    }
}

//******************************************************************
void quit() {
    // check if vectors/array are not NULL--if so, free each vector/array 	);
    if(resource != NULL) {
        free(resource);
    }
}


//*************************************************************
int main() {
    int choice = 0;

    while(choice != 5){
        printf("*********************************************************\n");
        printf("Welcome to Bankers Algorithm!\n\n");
        printf("Please choose one of the options below:\n");
        printf("1. Enter claim graph\n");
        printf("2. Request resource\n");
        printf("3. Release resource\n");
        printf("4. Determine safe sequence\n");
        printf("5. Quit\n");

        printf("Selection:");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                enter_claim_graph();
                break;
            case 2:
                request_resource();
                break;
            case 3:
                release_resource();
                break;
            case 4:
                safe_sequence(need, available);
                break;
        }
    }
    quit();
    printf("Have a great day!");
}
