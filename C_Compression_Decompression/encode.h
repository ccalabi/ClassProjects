# define LEFT 0
# define RIGHT 1

void printTree(treeNode *t, int depth);

void printTreeInOrder(treeNode *t, int depth);

void makeCodes(treeNode *node, code *table, code aCode);

static inline void spaces(int c)
{
    for (int i = 0; i < c; i++)
    {
        putchar(' ');
    }
    return;
}

