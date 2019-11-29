#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]) {
	FILE *fp1, *fp2;
	char *x, *y;
	int xlen, ylen, **dp, i, j, m;
	x = (char*)malloc(sizeof(char) * 10000000);
	y = (char*)malloc(sizeof(char) * 10000000);
	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");
	fscanf(fp1, "%s", x);
	fscanf(fp2, "%s", y);
	xlen = strlen(x);
	ylen = strlen(y);
	dp = (int**)malloc((xlen + 1) * sizeof(int**));
	for (i = 0; i <= xlen; i++) {
		dp[i] = (int*)malloc((ylen + 1) * sizeof(int*));
		dp[i][0] = i;
	}
	for (j = 0; j <= ylen; j++) {
		dp[0][j] = j;
	}
	for (i = 1; i <= xlen; i++) {
		for (j = 1; j <= ylen; j++) {
			if (dp[i][j - 1] < dp[i - 1][j])
				dp[i][j] = dp[i][j - 1] + 1;
			else
				dp[i][j] = dp[i - 1][j] + 1;
			if (x[i - 1] == y[j - 1])
				m = 0;
			else
				m = 1;
			if (dp[i - 1][j - 1] + m < dp[i][j])
				dp[i][j] = dp[i - 1][j - 1] + m;
		}
	}
	printf("%d\n", dp[xlen][ylen]);
	return 0;
}
