/*Joud Thaher
  1221381
  Section:2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUILDINGS 100
#define INFINITE 99999
#define MAX_COURSES 100

int loadBuilding = 0;
int loadCourse = 0;
int sorted=0;

typedef struct {
    int building;
    int distance;
} HeapNode;

typedef struct PriorityQueue* Heap;
struct  PriorityQueue{
    HeapNode *array;
    int capacity;
    int size;
};

int numOfBuildings = 0;
int buildingDistances[MAX_BUILDINGS][MAX_BUILDINGS];
char buildingNames[MAX_BUILDINGS][20];

int numOfCourses = 0;
int coursePrerequisites[MAX_COURSES][MAX_COURSES];
char courseNames[MAX_COURSES][20];

void displayMenu();
int findBuildingIndex(char*);
void loadBuildings();
void displayBuildings();
void initializeHeap(Heap,int);
void insert(Heap,int,int);
HeapNode deleteMin(Heap);
void dijkstra(int,int);
void displayShortestRoute(int,int,int[]);
void dijkstraRoute(int,int);
int getCourseIndex(char[]);
void loadCourses();
void displayCourseData();
void calculateInDegree(int[],int);
void topologicalSort();
void printTopologicalSort();

int main() {
    int choice;
    printf("Welcome!! :))");

    do {
        displayMenu();
        printf("Please enter your choice:");
        scanf("%d", &choice);
        getchar();

        switch (choice) {

            case 1:
                printf("Loading data from the files...\n");
                loadBuildings();
                loadCourses();
                break;

            case 2:
                printf("Calculating the shortest distance between the buildings...\n");

                if(loadBuilding==0){
                    printf("You have NOT loaded the buildings file:( \n");
                }

                else {

                    char sourceBuilding[20], destinationBuilding[20];
                    printf("Enter source building name:");
                    scanf("%s", sourceBuilding);
                    printf("Enter destination building name:");
                    scanf("%s", destinationBuilding);

                    int sourceIndex = findBuildingIndex(sourceBuilding);
                    int destinationIndex = findBuildingIndex(destinationBuilding);

                    if (sourceIndex == -1 || destinationIndex == -1) {
                        printf("Invalid building names! Please enter valid building names.\n");
                        break;
                    }

                    dijkstra(sourceIndex, destinationIndex);
                }

                break;

            case 3:
                printf("Printing the shortest route between the buildings...\n");

                if(loadBuilding==0){
                    printf("You have NOT loaded the buildings file:( \n");
                }

                else {

                    char sourceBuilding[20], destinationBuilding[20];
                    printf("Enter source building name:");
                    scanf("%s", sourceBuilding);
                    printf("Enter destination building name:");
                    scanf("%s", destinationBuilding);

                    int sourceIndex = findBuildingIndex(sourceBuilding);
                    int destinationIndex = findBuildingIndex(destinationBuilding);

                    if (sourceIndex == -1 || destinationIndex == -1) {
                        printf("Invalid building names! Please enter valid building names.\n");
                        break;
                    }
                    dijkstraRoute(sourceIndex, destinationIndex);
                }
                break;

            case 4:
                printf("Sorting the courses using the topological sort...\n");

                if(loadCourse==0){
                    printf("You have NOT loaded the courses file!\n");
                }
                else{
                    topologicalSort();
                    printf("Sorted successfully according to topological sort!\n");
                    sorted=1;
                }
                break;

            case 5:
                printf("Printing the sorted courses...\n");

                if(loadCourse==0){
                    printf("You have NOT loaded the courses file!\n");
                }
                else {
                    if(sorted==0){
                        printf("You have NOT sorted them. Enter 4 to sort!\n");
                    }
                    else{
                        printTopologicalSort();
                    }
                }
                break;

            case 6:
                printf("Exiting the application...\n");
                break;

            default:
                printf("Invalid choice! :( Please reread the menu and enter a valid option.\n");
                break;

        }

    } while (choice != 6);

    return 0;
}

void displayMenu() {
    printf("\n !! Course Prerequisite Planner and Campus Route Finder !! \n");
    printf("1. Load the two files.\n");
    printf("2. Calculate the shortest distance between two buildings.\n");
    printf("3. Print the shortest route between two buildings and the total distance.\n");
    printf("4. Sort the courses using the topological sort.\n");
    printf("5. Print the sorted courses.\n");
    printf("6. Exit the application.\n");
}

int findBuildingIndex(char *name) {

    for (int i = 0; i < numOfBuildings; i++) {
        if (strcasecmp(buildingNames[i], name) == 0) {
            return i; //return index if found
        }
    }
    return -1; //not found
}

void loadBuildings() {

    FILE *in= fopen("input_buildings.txt", "r");

    if (in == NULL) {
        printf("File does not exist!\n");
        return;
    }

    //initialize distances to inf  (helps with dijkstra)
    for (int i = 0; i < MAX_BUILDINGS; i++) {
        for (int j = 0; j < MAX_BUILDINGS; j++) {
            buildingDistances[i][j] = INFINITE;
        }
    }

    char line[100];

    while (fgets(line, sizeof(line), in)) {

        char building1[20], building2[20];
        int distance;

        char *token = strtok(line, "#");
        strcpy(building1, token);

        token = strtok(NULL, "#");
        strcpy(building2, token);

        token = strtok(NULL, "#");
        distance = atoi(token); //function that turns strings into numbers

        // Find or add the index of the first building
        int index1 = findBuildingIndex(building1);
        if (index1 == -1) {
            strcpy(buildingNames[numOfBuildings], building1);
            index1 = numOfBuildings++;
        }

        // Find or add the index of the second building
        int index2 = findBuildingIndex(building2);
        if (index2 == -1) {
            strcpy(buildingNames[numOfBuildings], building2);
            index2 = numOfBuildings++;
        }

        // Update the distance between the two buildings
        buildingDistances[index1][index2] = distance;
        buildingDistances[index2][index1] = distance; // undirected graph
    }

    fclose(in);
    printf("Loaded building distances successfully!\n");
    loadBuilding = 1;
    //displayBuildings();
}

void displayBuildings() { //just to make sure it stores the information correctly

    printf("Number of Buildings: %d\n", numOfBuildings);
    printf("Building Names:\n");

    for (int i = 0; i < numOfBuildings; i++) {
        printf("%s ", buildingNames[i]);
    }

    printf("\nBuilding Distances:\n");

    for (int i = 0; i < numOfBuildings; i++) {
        for (int j = 0; j < numOfBuildings; j++) {
            if (buildingDistances[i][j] == INFINITE) {
                printf("INF ");
            }
            else {
                printf("%d ", buildingDistances[i][j]);
            }
        }
        printf("\n");
    }
}

//initialize priority queue (heap) to use for dijkstra
void initializeHeap(Heap heap, int capacity) {
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (HeapNode *)malloc(capacity * sizeof(HeapNode));
}

//insert into the heap
void insert(Heap heap, int building, int distance) {

    //checks whether it is full
    if (heap->size == heap->capacity) {
        return;
    }

    int i = heap->size++;
    heap->array[i].building = building;
    heap->array[i].distance = distance;

    while (i > 0 && heap->array[(i - 1) / 2].distance > heap->array[i].distance) { //heap equations for priority

        HeapNode temp = heap->array[i];
        heap->array[i] = heap->array[(i - 1) / 2];
        heap->array[(i - 1) / 2] = temp;
        i = (i - 1) / 2;

    }
}

HeapNode deleteMin(Heap heap) {

    //checks whether it is empty
    if (heap->size == 0) {
        HeapNode invalid = {-1, -1};
        return invalid;
    }

    HeapNode min = heap->array[0];
    heap->array[0] = heap->array[--heap->size];

    int i = 0;
    while (1) {

        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int smallest = i;

        if (leftChild < heap->size && heap->array[leftChild].distance < heap->array[smallest].distance) {
            smallest = leftChild;
        }

        if (rightChild < heap->size && heap->array[rightChild].distance < heap->array[smallest].distance) {
            smallest = rightChild;
        }

        if (smallest != i) {

            HeapNode temp = heap->array[i];
            heap->array[i] = heap->array[smallest];
            heap->array[smallest] = temp;
            i = smallest;

        }
        else {
            break;
        }
    }

    return min;
}

void dijkstra(int source, int destination) {

        int distance[numOfBuildings];
        int previous[numOfBuildings];
        int visited[numOfBuildings];

        for (int i = 0; i < numOfBuildings; i++) {
            distance[i] = INFINITE;
            previous[i] = -1;
            visited[i] = 0;
        }

        distance[source] = 0;
        Heap heap;
        initializeHeap(heap, numOfBuildings);
        insert(heap, source, 0);

        while (heap->size > 0) {

            HeapNode min = deleteMin(heap);
            int u = min.building;
            visited[u] = 1;

            for (int v = 0; v < numOfBuildings; v++) {
                if (buildingDistances[u][v] < 0) {//if a distance was negative in the file
                    printf("Error: Negative distance from %s to %s.\nPlease make sure all distances are positive\n", buildingNames[u], buildingNames[v]);
                    return;
                }
                if (!visited[v] && buildingDistances[u][v] != INFINITE) {
                    int alternative = distance[u] + buildingDistances[u][v];
                    if (alternative < distance[v]) {
                        distance[v] = alternative;
                        previous[v] = u;
                        insert(heap, v, alternative);
                    }
                }
            }
        }

        printf("\nShortest distance from %s to %s: %d\n", buildingNames[source], buildingNames[destination], distance[destination]);

}

void dijkstraRoute(int source, int destination) { //same as dijkstra but it prints the route too

    int distance[numOfBuildings];
    int previous[numOfBuildings];
    int visited[numOfBuildings];

    for (int i = 0; i < numOfBuildings; i++) {
        distance[i] = INFINITE;
        previous[i] = -1;
        visited[i] = 0;
    }

    distance[source] = 0;
    Heap heap;
    initializeHeap(heap, numOfBuildings);
    insert(heap, source, 0);

    while (heap->size > 0) {

        HeapNode min = deleteMin(heap);
        int u = min.building;
        visited[u] = 1;

        for (int v = 0; v < numOfBuildings; v++) {
            if (buildingDistances[u][v] < 0) {//if a distance was negative in the file
                printf("Error: Negative distance from %s to %s.\nPlease make sure all distances are positive\n", buildingNames[u], buildingNames[v]);
                return;
            }
            if (!visited[v] && buildingDistances[u][v] != INFINITE) {
                int alternative = distance[u] + buildingDistances[u][v];
                if (alternative < distance[v]) {
                    distance[v] = alternative;
                    previous[v] = u;
                    insert(heap, v, alternative);
                }
            }
        }
    }

    printf("\nShortest route:");
    displayShortestRoute(source,destination, previous);
    printf("\nTotal distance from %s to %s: %d\n", buildingNames[source], buildingNames[destination], distance[destination]);

}

void displayShortestRoute(int source, int destination, int previous[]) { //prints the shortest route

    if (source == destination) {
        printf("%s ", buildingNames[source]);
    }
    else if (previous[destination] == -1) {
        printf("No route exists.\n");
    }
    else {
        displayShortestRoute(source, previous[destination], previous);
        printf("-> %s ", buildingNames[destination]);
    }
}

int getCourseIndex(char course[]) { //find the index of the course

    for (int i = 0; i < numOfCourses; i++) {
        if (strcmp(courseNames[i], course) == 0) {
            return i;
        }
    }
    return -1; //not found
}

void loadCourses() {

    FILE *in = fopen("input_courses.txt", "r");

    if (in == NULL) {
        printf("File does not exist!\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), in)) {
        char *course = strtok(line, "#\n");
        int courseIndex = getCourseIndex(course);

        if (courseIndex == -1) {
            courseIndex = numOfCourses;
            strcpy(courseNames[numOfCourses++], course);
        }

        char *prerequisite;
        while ((prerequisite = strtok(NULL, "#\n")) != NULL) {
            int prerequisiteIndex = getCourseIndex(prerequisite);

            if (prerequisiteIndex == -1) {
                prerequisiteIndex = numOfCourses;
                strcpy(courseNames[numOfCourses++], prerequisite);
            }

            coursePrerequisites[courseIndex][prerequisiteIndex] = 1;
        }
    }

    fclose(in);
    printf("Loaded courses and prerequisites successfully!\n");
    loadCourse=1;
    //displayCourseData();
}

void displayCourseData() { //just to make sure it stores the information correctly

    if (numOfCourses == 0) {
        printf("No courses loaded!\n");
        return;
    }

    printf("Number of Courses: %d\n", numOfCourses);
    printf("Course Names and Prerequisites:\n");

    for (int i = 0; i < numOfCourses; i++) {
        printf("%s: ", courseNames[i]);
        int hasPrerequisites = 0;
        for (int j = 0; j < numOfCourses; j++) {
            if (coursePrerequisites[i][j] == 1) {
                if (!hasPrerequisites) {
                    printf("Prerequisites -> ");
                    hasPrerequisites = 1;
                }
                printf("%s ", courseNames[j]);
            }
        }
        if (!hasPrerequisites) {
            printf("No prerequisites");
        }
        printf("\n");
    }

    printf("\nAdjacency Matrix:\n   ");
    for (int i = 0; i < numOfCourses; i++) {
        printf("%3d", i);
    }
    printf("\n");

    for (int i = 0; i < numOfCourses; i++) {
        printf("%3d", i);
        for (int j = 0; j < numOfCourses; j++) {
            printf("%3d", coursePrerequisites[i][j]);
        }
        printf("\n");
    }
}

void calculateInDegree(int inDegree[], int n) { //useful for topological sort

    for (int i = 0; i < n; i++) {
        inDegree[i] = 0;
        for (int j = 0; j < n; j++) {
            if (coursePrerequisites[j][i] == 1) {
                inDegree[i]++;
            }
        }
    }

}
void topologicalSort() { //sorting using topological sort

    int inDegree[MAX_COURSES];
    calculateInDegree(inDegree, numOfCourses);

    int queue[MAX_COURSES];
    int front = 0, rear = 0;

    for (int i = 0; i < numOfCourses; i++) {
        if (inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }

    int sortedOrder[MAX_COURSES];
    int index = 0;

    while (front < rear) {

        int minIndex = queue[front];

        for (int i = front + 1; i < rear; i++) {
            if (queue[i] < minIndex) {
                minIndex = queue[i];
            }
        }

        for (int i = front; i < rear; i++) {

            if (queue[i] == minIndex) {

                int temp = queue[front];
                queue[front] = queue[i];
                queue[i] = temp;

                int course = queue[front++];
                sortedOrder[index++] = course;

                for (int j = 0; j < numOfCourses; j++) {

                    if (coursePrerequisites[course][j] == 1 && --inDegree[j] == 0) {
                        queue[rear++] = j;
                    }
                }

                break;
            }
        }
    }

    if (index != numOfCourses) {
        printf("There exists a cycle in the graph. Topological sorting not possible.\n");
        return;
    }

}

void printTopologicalSort() { //same as topologicalSort but with printing the sorted courses

    int inDegree[MAX_COURSES];
    calculateInDegree(inDegree, numOfCourses);

    int queue[MAX_COURSES];
    int front = 0, rear = 0;

    for (int i = 0; i < numOfCourses; i++) {
        if (inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }

    int sortedOrder[MAX_COURSES];
    int index = 0;

    while (front < rear) {

        int minIndex = queue[front];

        for (int i = front + 1; i < rear; i++) {
            if (queue[i] < minIndex) {
                minIndex = queue[i];
            }
        }

        for (int i = front; i < rear; i++) {

            if (queue[i] == minIndex) {

                int temp = queue[front];
                queue[front] = queue[i];
                queue[i] = temp;

                int course = queue[front++];
                sortedOrder[index++] = course;

                for (int j = 0; j < numOfCourses; j++) {

                    if (coursePrerequisites[course][j] == 1 && --inDegree[j] == 0) {
                        queue[rear++] = j;
                    }
                }

                break;
            }
        }
    }

    if (index != numOfCourses) {
        printf("There exists a cycle in the graph. Topological sorting not possible.\n");
        return;
    }

    printf("\nTopological Sort Order:\n");
    for (int i = index - 1; i >= 0; i--) {
        printf("%s\n", courseNames[sortedOrder[i]]);
    }
}