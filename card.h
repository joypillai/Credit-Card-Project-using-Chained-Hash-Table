struct Card{
	char First_Name [16];
        char Middle_Name [16];
        char Last_Name [16];
	char DOB [8];
	char Identification [16];
	char Line[32];
        char City[24];
        char State[18];
        int Pin;
	char Email [72];
	unsigned long long Card_Num;
	char Card_Exp [8];
	char Card_Issue [8];
	char CVV[3];
	struct Card * next;
};
