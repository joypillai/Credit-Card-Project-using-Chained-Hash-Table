
struct node{
        char *proof;
        char *address;
        char *fullName;
        char *email;
        unsigned long long int cardNo;
        char *bank;
        char *namePrint;
        struct node *next;
        int dob;
        int cardExp;
        int cardIssue;
        int cvv;
}node;
