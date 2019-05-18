#define BLOCK_SIZE 16

__kernel void cblas_sgemm(const int m, const int n, const int k,
                          const __global float *a,
                          const __global float *b,
                          __global float *c) {

    const int rows = get_global_id(0); // Rows
    const int cols = get_global_id(1); // Cols

    float sum = 0.f;
    for (int i = 0; i < n; i++) {
        sum += a[rows*n + i]*b[i*k + cols];
    }

    c[rows*k + cols] = sum;
}


__kernel void cblas_sgemm_optimized(const int m, const int n, const int k,
                          const __global float *a,
                          const __global float *b,
                          __global float *c) {

    const int row = get_local_id(0);
    const int col = get_local_id(1);
    const int globalRow = BLOCK_SIZE*get_group_id(0) + row;
    const int globalCol = BLOCK_SIZE*get_group_id(1) + col;

    __local float Asub[BLOCK_SIZE][BLOCK_SIZE];
    __local float Bsub[BLOCK_SIZE][BLOCK_SIZE];

    float acc = 0.0f;

    const int numTiles = k/BLOCK_SIZE;
    for (int t = 0; t < numTiles; t++) {

        const int tiledRow = BLOCK_SIZE*t + row;
        const int tiledCol = BLOCK_SIZE*t + col;
        Asub[col][row] = a[globalRow*n + tiledCol];
        Bsub[col][row] = b[tiledRow*k + globalCol];

        barrier(CLK_LOCAL_MEM_FENCE);
        for (int i = 0; i < BLOCK_SIZE; i++) {
            acc += Asub[i][row] * Bsub[col][i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    c[globalRow*k + globalCol] = acc;
}

