
/*
 *
 * COP 3502C Final term assignment 1
 * This program is written by: Kensal J Ramos
 * 
 * Description: Given a circle with radius r and a list of points determine if the points lie on the circle. 
 * The ones that do lie on the circle, sort them using a merge sort. Then search through them using a binary
 * search.
 * 
 * Required functions:
 * 1) ReadData() - This function reads the data from the in.txt file and returns the data 
 * 2) FilterData() - This function takes the data received from ReadData() and returns filtered points that are
 *                   inside or on the circle line
 * 3) MergeSort() - 
 * 4) Merge() - 
 * 5) BinarySearch() - 
 * 6) SearchPhase() - This functions takes sorted data modified by merge sort. Then continuously prompts the user
 *                    for search points.
 * 
 */ 

// Pre-Processor directives
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structures

// Function prototypes
int ** readData(int *r, int **arr, int *numOfPoints, int *x, int *y);
int ** filterData(int **arr, int numOfPoints, int r, int h, int k);
void mergeSort(int **arr, int l, int r);
void saveData(int **arr, int numOfPoints);
void merge(int **arr, int l, int mid, int r);
int getArrLength(int **arr);
int charToInt(char *arr);
void searchPhase(int **arr, int numOfPoints);
int binarySearch(int **arr, int x, int y, int numOfPoints);

int main(void) {

    int x, y, r, **arr, numOfPoints, numOfInsidePoints;

    arr = readData(&r, arr, &numOfPoints, &x, &y);
    arr = filterData(arr, numOfPoints, r, x, y);
    numOfInsidePoints = getArrLength(arr);
    mergeSort(arr, 0, numOfInsidePoints - 1);
    saveData(arr, numOfInsidePoints);
    printf("\nFiltered and sorted data written to out.txt");
    searchPhase(arr, numOfInsidePoints);


    return 0;
}

/*
 *
 * Name: readData();
 * Parameters:
 *              int *r           - The radius of the given circle
 *              int **arr        - The array the program is using to store the given points
 *              int *numOfPoints - The number of points in the in.txt file
 *              int *x           - The x coordinate of the center of the cirlce
 *              int *y           - The y coordinate of the center of the circle
 * 
 * Description: This function will read from the in.txt file and get the following information: 
 * Center of the circle (h, k), radius of the circle, number of points, list of points
 * Runtime: O(n)
 * 
 */
int ** readData(int *r, int **arr, int *numOfPoints, int *x, int *y) {

    FILE *fp;
    char buff[255];

    int a, b;

    fp  = fopen("./in.txt", "r");

    // X Value
    fscanf(fp, "%s", buff); 
    *x = charToInt(buff);

    // Y Value
    fscanf(fp, "%s", buff);
    *y = charToInt(buff);

    // Radius
    fscanf(fp, "%s", buff);
    *r = charToInt(buff);

    // Points to test
    fscanf(fp, "%s", buff);
    *numOfPoints = charToInt(buff);
    arr = (int **) malloc(sizeof(int *) * *numOfPoints);

    for (int i = 0; i < *numOfPoints; i++) {

        arr[i] = (int *) malloc(sizeof(int) * 2);
        // Set x coordinate
        fscanf(fp, "%s", buff);
        arr[i][0] = charToInt(buff);

        // Set y coordinate
        fscanf(fp, "%s", buff);
        arr[i][1] = charToInt(buff);

        printf("\nAdded (%d,%d) to array.", arr[i][0], arr[i][1]);

    }

    fclose(fp);
    return arr;
}

/*
 *
 * Name: filterData();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 *              int numOfPoints - The number of points initially given
 *              int r           - Represents the radius of the given circle
 * 
 * Description: This function will verify that the points are in the circle. It does this by the fact
 * that (x-h)^2 + (y-k)^2 = r^2. Where h,k are the center coordinates of the circle
 * Runtime: O(n)
 * 
 */
int ** filterData(int **arr, int numOfPoints, int r, int h, int k) {

    int x, y, equation;
    int **newArr;
    int i;
    int count = 0;

    newArr = (int **) malloc(sizeof(int *));

    for (i = 0; i < numOfPoints; i++) {

        x = arr[i][0];
        y = arr[i][1];
        equation = pow(x - (h), 2) + pow(y - (k), 2);
        if (equation <= pow(r, 2)) {
            newArr[count] = (int *) malloc(sizeof(int) * 2);

            newArr[count][0] = arr[i][0];
            newArr[count][1] = arr[i][1];

            printf("\n(%d,%d) is inside/on the circle.", newArr[count][0], newArr[i][1]);
            count++;

        }

    }

    //This will be our sentinel value to get num of points
    newArr[count] = (int *) malloc(sizeof(int) * 2);
    newArr[count][0] = -999;

    // Free
    for (int i = 0; i < numOfPoints; i++) 
        free(arr[i]);
    free(arr);

    return newArr;

}

/*
 *
 * Name: charToInt();
 * Parameters:
 *              int *str        - The char array we are converting to an integer
 * 
 * Description: This function will convert character arrays to integers
 * Runtime: O(n)
 * 
 */
int charToInt(char *str) {

    int i = 0, result = 0, length = 0, negFlag = 0;

    // Get length
    while(str[length] != '\0') 
        length++;
    
    // Convert to integer
    while(str[i] != '\0') {

        if (str[i] == '-')
            negFlag = 1;
        else 
            result += (str[i] - '0') * pow(10, length - 1);
        i++;
        length--;
    }
    
    if (negFlag == 1)
        result *= -1;

    return result;
}

/*
 *
 * Name: mergeSort();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 *              int l           - Represents the left of an array
 *              int r           - Represents the right of an array
 * 
 * Description: This function will split the array by calculating a middle index. 
 * The merge() function will do the sorting 
 * Runtime: O(log(n)) (Since T(n) = T(n/2) + 1 )
 * 
 */


void mergeSort(int **arr, int l, int r) {

    int mid = l + (r - 1) / 2;

    if (l < r) {

        mergeSort(arr, l, mid);
        mergeSort(arr, mid + 1, r);
        
        merge(arr, l, mid, r);

    }

}

/*
 *

 */ 

/*
 *
 * Name: merge();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 *              int l           - Represents the left of an array
 *              int mid         - Represents where to "split" an array
 *              int r           - Represents the right of an array
 * 
 * Description: This function will use 3 variables, i, j, k. i represents the index of the left array, while j
 * represents the index of the right array. k will hold the place in the actual array, or in this
 * case, the place of 'l'. 
 * Runtime: O(n) (There are 4 unnested loops, so 4n)
 * 
 */
void merge(int **arr, int l, int mid, int r) {

    int i, j, k;

    int leftLen = mid - l + 1; // + 1 to account for 0 index
    int rightLen = r - mid;

    // Temp-arrays
    int leftArr[leftLen][2];
    int rightArr[rightLen][2];

    // Place values into our temp arrays
    for (i = 0; i < leftLen; i++) {
        leftArr[i][0] = arr[l + i][0];
        leftArr[i][1] = arr[l + i][1];
    }

    for (j = 0; j < rightLen; j++) {
        rightArr[j][0] = arr[mid + 1 + j][0];
        rightArr[j][1] = arr[mid + 1 + j][1];  
    }

    // Reset values and set k = l because thats the first index of the array we want to merge
    i = 0;
    j = 0; 
    k = l; 

    // Swap necessary values
    while (i < leftLen && j < rightLen) {

        // If x's are the same, test the y values
        if (leftArr[i][0] == rightArr[j][0]) {

            if (leftArr[i][1] <= rightArr[j][1]) {
                arr[k][0] = leftArr[i][0];
                arr[k][1] = leftArr[i][1];
                i++;
            }
            else {
                arr[k][0] = rightArr[j][0];
                arr[k][1] = rightArr[j][1];
                j++;
            }
        }
        else if (leftArr[i][0] < rightArr[j][0]) {
            arr[k][0] = leftArr[i][0];
            arr[k][1] = leftArr[i][1];
            i++;
        } 
        else {
            arr[k][0] = rightArr[j][0];
            arr[k][1] = rightArr[j][1];
            j++;
        }

        k++;
    }

    // There may still be elements remaining inside the arrays, so we can copy them in
    for (; i < leftLen; i++, k++) {
        arr[k][0] = leftArr[i][0];
        arr[k][1] = leftArr[i][1];
    }

    for (; j < rightLen; j++, k++) {
        arr[k][0] = rightArr[j][0];
        arr[k][1] = rightArr[j][1];
    }

}

/*
 *
 * Name: saveData();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 *              int numOfPoints - Number of points in the array (arr)
 * 
 * Description: Opens the out.txt file for writing. Then it writes all points in the array to the file.
 * Runtime: O(n)
 * 
 */
void saveData(int **arr, int numOfPoints) {

    FILE *fp;

    fp  = fopen("./out.txt", "w");

    for (int i = 0; i < numOfPoints; i++) 
        fprintf(fp, "(%d, %d)\n", arr[i][0], arr[i][1]);
    


    fclose(fp);
}

/*
 *
 * Name: getArrLength();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 * 
 * Description: Gets length of array (arr).
 * Runtime: O(n)
 * 
 */
int getArrLength(int **arr) {

    int i = 0;

    while (arr[i][0] != -999)
        i++;

    return i;
}

/*
 *
 * Name: searchPhase();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 *              int numOfPoints - Number of points in the array (arr)
 * 
 * Description: This function takes input from the user and searches for that x,y coordinate. -999 exits the program.
 * Runtime: O(n) (n being the amount of times the user runs it)
 * 
 */
void searchPhase(int **arr, int numOfPoints) {

    int x, y, num;

    while (x != -999 || y != -999) {

        printf("\nPlease enter an x-value (-999 to exit): ");
        scanf("%d", &x);
        if (x == -999) {
            printf("exiting...");
            break;
        }
        printf("\nPlease enter a y value: ");
        scanf("%d", &y);
        num = binarySearch(arr, x, y, numOfPoints);

        if (num != -1) 
            printf("\nPoint found! Located at index %d. (%d, %d)", num, x, y);
        else if (num == -1)
            printf("\nPoint not found!");

    }

}

/*
 *
 * Name: binarySearch();
 * Parameters:
 *              int **arr       - The main array containing filtered points
 *              int x           - x value the user inputted
 *              int y           - y value the user inputted
 *              int numOfPoints - Number of points in the array (arr)
 * 
 * Description: This function uses a high and a low value to "split" the array in half therefore reduing runtime
 * Runtime: O(n/2)
 * 
 */
int binarySearch(int **arr, int x, int y, int numOfPoints) {

    int l = 0, h = numOfPoints - 1;
    int mid;

    while (l <= h && x != -999 && y != -999) {

        mid = (l + h) / 2;

        if (x == arr[mid][0] && y == arr[mid][1]) 
            return mid;
        else if (x < arr[mid][0]) 
            h = mid - 1;
        else
            l = mid + 1;

    }

    return -1;

}