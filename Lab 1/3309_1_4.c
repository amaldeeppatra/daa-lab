#include <stdio.h>

void ROTATE_RIGHT(int *arr, int p2) {
    if (p2 <= 1) return;
    int last = arr[p2 - 1];
    for (int i = p2 - 1; i > 0; i--)
        arr[i] = arr[i - 1];
    arr[0] = last;
}

int main() {
    int N, p2;

    printf("Enter the size of the array (N): ");
    scanf("%d", &N);
    int arr[N];

    printf("Enter %d elements of the array: ", N);
    for (int i = 0; i < N; i++)
        scanf("%d", &arr[i]);

    printf("Enter the number of elements to rotate right: ");
    scanf("%d", &p2);

    printf("Before ROTATE: ");
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    ROTATE_RIGHT(arr, p2);

    printf("After ROTATE: ");
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
