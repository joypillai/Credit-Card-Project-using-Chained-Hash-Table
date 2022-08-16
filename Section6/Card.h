struct Card{
  char First_Name[20];
  char Last_Name[20];
  char Middle_Name[20];
   char DOB[10];
   char Identification[20];
   char Line[20];
   char City[72];
   char State[16];
   int Pin;
   char Email[42];
   unsigned long long Card_Num;
   char Card_Exp[8];
   char Card_Issue[8];
   
   char CVV[3];
  
   struct card* next;
};
