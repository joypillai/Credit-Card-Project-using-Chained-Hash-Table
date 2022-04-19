struct Address{
        char Line[32];
        char City[24];
        char State[18];
        int Pin;
};

struct Full_Name{
        char First_Name [16];
        char Middle_Name [16];
        char Last_Name [16];
};

struct Card{
	struct Full_Name * Name;
	char DOB [8];
	char Identification [16];
	struct Address * Addr;
	char Email [72];
	unsigned long long Card_Num;
	char Card_Exp [8];
	char Card_Issue [8];
	char CVV[3];
};

struct Bucklist{
	
	struct Card * ListCard;
	struct Bucklist * next;
};
