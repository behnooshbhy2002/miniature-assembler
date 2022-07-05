#include "main.h"
void main(int argc,char **argv)
{
    FILE *input , *output , *fopen();
    struct symbolTable *SymTab; //SymTab
    int symbolTableLen;
    int i , j , NumberOfInst;
    struct instruction *currentInst;
    size_t lineSize=100;
    char *line;
    char *token;
    char *instructions[]={"add" , "sub" , "slt" , "or" , "nand" , "addi" ,
                          "slti" , "ori" , "lui" , "lw" , "sw" , "beq" , "jalr" , "j" , "halt"};
    int instCount=0;
    line = (char *)malloc(72);
    currentInst = (struct instruction *) malloc(sizeof (struct instruction));

    if(argc < 3)
    {
        printf("***** Please run this program as follows:\n");
        printf("***** %s test.as test.mc\n",argv[0]);
        printf("***** where test.as is your assembly program\n");
        printf("***** and test.mc will be your machine code.\n");
        exit(1);
    }
    if((input=fopen(argv[1],"r")) == NULL)
    {
        printf("%s cannot be openned\n",argv[1]);
        exit(1);
    }
    if((output=fopen(argv[2],"w+")) == NULL)
    {
        printf("%s cannot be openned\n",argv[2]);
        exit(1);
    }

    symbolTableLen = findSymbolTableLen(input);

    SymTab = (struct symbolTable *) malloc(symbolTableLen*sizeof (struct symbolTable));

    for(i=0; i<symbolTableLen; i++)
    {
        SymTab[i].symbol=(char *) malloc(symbolTableLen*sizeof (struct symbolTable));
    }

    NumberOfInst = fillSymTab(SymTab , input , output);

    check_labels(symbolTableLen , SymTab , output);

    printf("%s\n" , "Symbol Table : ");
    for (int (i) = 0; (i) < symbolTableLen; ++(i))
    {
        printf("%s %d\n" , SymTab[i].symbol , SymTab[i].value);
    }

    printf(" %-3s%-6s |%-6s%-10s | %-16s%-16s\n" , "" , "Hexa" , "" , "Decimal" , "" ,"Binary");
    printf("----------------------------------------------------------------------\n");


    while (fgets(line , lineSize , input) != NULL)
    {
        bool IsValidOpcode = false;
        bool IsLabel = false;
        instCount++;
        token = strtok(line , "\t ,\n");

        for(i=0; i<symbolTableLen; i++)
        {
            if(strcmp(SymTab[i].symbol , token)==0)
            {
                token= strtok(NULL , "\t ,\n");
            }
        }

        // check ".fill"
        if(strcmp(token , ".fill")==0)
        {
            currentInst->instType=-1;
            token = strtok(NULL , "\t ,\n");
            currentInst->op=0;
            currentInst->Instdecimal = FindImmValue(token , SymTab , symbolTableLen , output);
            currentInst->imm = currentInst->Instdecimal;
            IsValidOpcode = true;
        }

        else
        {

         for(j=0; j<15; j++)
         {
            if(strcmp(instructions[j] , token )==0)
            {
                IsValidOpcode = true;
                currentInst->op = j;

                    if(j>-1 && j<5) //R format
                    {
                        currentInst->instType=0;

                        char *rd=strtok(NULL ,"\t ,\n");
                        currentInst->rd = atoi(rd);

                        char *rs=strtok(NULL ,"\t ,\n");
                        currentInst->rs = atoi(rs);

                        char *rt=strtok(NULL ,"\t ,\n");
                        currentInst->rt = atoi(rt);

                    }
                    else if(j>4 && j<13) //I Format
                    {
                        currentInst->instType=1;

                        if(j>4 && j<8) //addi , slti , ori
                        {
                            char *rt=strtok(NULL ,"\t ,");
                            currentInst->rt=atoi(rt);

                            char *rs=strtok(NULL ,"\t ,");
                            currentInst->rs=atoi(rs);

                            char *offset=strtok(NULL ,"\t ,\n");
                            currentInst->imm = FindImmValue(offset , SymTab , symbolTableLen , output);

                            if(j==7)
                                currentInst->imm = abs(currentInst->imm);
                        }

                        else if(j==8) //lui
                        {
                            char *rt=strtok(NULL ,"\t ,\n");
                            currentInst->rt=atoi(rt);

                            currentInst->rs=0;

                            char *offset=strtok(NULL ,"\t ,\n");
                            currentInst->imm = FindImmValue(offset , SymTab , symbolTableLen , output);
                        }

                        else if(j>8 && j<12) //lw , sw , beq
                        {
                            char *rt=strtok(NULL ,"\t ,");
                            currentInst->rt=atoi(rt);

                            char *rs=strtok(NULL ,"\t ,");
                            currentInst->rs=atoi(rs);

                            char *offset=strtok(NULL ,"\t ,\n");
                            currentInst->imm = FindImmValue(offset , SymTab , symbolTableLen , output);

                            if(j==11)
                            {
                                currentInst->imm = currentInst->imm - instCount;
                            }

                        }
                        else if(j==12) //jalr
                        {
                            char *rt=strtok(NULL ,"\t ,");
                            currentInst->rt=atoi(rt);

                            char *rs=strtok(NULL ,"\t ,\n");
                            currentInst->rs=atoi(rs);

                            currentInst->imm=0;
                        }

                    }
                    else //J format
                    {
                    currentInst->instType=2;
                        if(j==13)
                        {
                            char *offset=strtok(NULL ,"\t ,\n");
                            currentInst->imm = FindImmValue(offset , SymTab , symbolTableLen , output);
                            currentInst->imm = abs(currentInst->imm);

                        }
                        else if(14) //halt
                        {
                            currentInst->imm=0;
                        }

                    }

            }

         }

        }

        // check register is more then 15
        if(currentInst->rd > 15 || currentInst->rd < 0)
        {
            currentInst->rd=0;
        }
        if(currentInst->rs > 15 || currentInst->rs < 0)
        {
            currentInst->rs=0;
        }
        if(currentInst->rt > 15 || currentInst->rt < 0)
        {
            currentInst->rt=0;
        }

        //check offset is not more than 65536
        if(currentInst->instType!=0 &&(currentInst->imm >= 65536 || currentInst->imm <= -65536))
        {
            fprintf(output , "%s\n" , "Offset Overflow.");
            printf("Offset Overflow.\n");
            exit(1);
        }
        if(!IsValidOpcode)
        {
            fprintf(output , "%s\n" , "Invalid Opcode.");
            printf("Invalid Opcode.\n");
            exit(1);
        }
        else
        {
            IntToHex(currentInst);
            currentInst->Instdecimal = HexToInt(currentInst->Hex);

            //print hex form
            printf("0x");
            for(int k=0; k<8; k++)
            {
               printf("%c" , currentInst->Hex[k]);
            }
            printf(" | ");

            //print Dec form
            printf("%-15d" , currentInst->Instdecimal);
            printf(" | ");

            //print Binary form
            Print_Binary(currentInst);

            //save decimal of instruction in file
            fprintf(output , "%d\n" , currentInst->Instdecimal);

        }

    }
    fclose(input);
    fclose(output);
}
int HexToInt(char *Hex)
{
    int sum=0;

      for(int i=0; i<8; i++)
      {
          if( ('0'<=Hex[i]) && (Hex[i]<='9') )
          {
              sum = sum * 16 + Hex[i] - '0';
          }
          else if(('A'<=Hex[i]) && (Hex[i]<='F'))
          {
              sum = sum * 16 + Hex[i] - 'A' + 10;
          }
      }

      return sum;
}
void decimalToHexa(int imm , char *Offset )
{
    int i = 0 ;
    long unsigned int n = 4294967295;
    if(imm<0)
    {
        imm+=1;
        n+=imm;
    }
    else
    {
        n = imm;
    }
    while (n != 0) {
        int temp = 0;
        temp = n % 16;
        if (temp < 10) {
            Offset[i] = temp + 48;
        }
        else {
            Offset[i] = temp + 55;
        }
        i++;
        n = n / 16;

    }
    Offset[i]=NULL;
}
void IntToHex(struct instruction *inst)
{
    int c;
   char hexTable[16]={'0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' ,
                        '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F'};

   for(int i=0; i<8; i++)
   {
        inst->Hex[i]='0';
   }

   inst->Hex[1] = hexTable[inst->op];

   if(inst->instType==-1)
   {
       int temp = inst->Instdecimal;
       char Hexa[8] = {'0'};

       decimalToHexa(temp , Hexa);
       int length = strlen(&Hexa);

       int c = 8-length;
       for(int i=length-1; i>=0; i--)
       {
           inst->Hex[c] = Hexa[i];
           c++;
       }

   }
   else if(inst->instType==0)//R Type
   {

       inst->Hex[2] = hexTable[inst->rs];
       inst->Hex[3] = hexTable[inst->rt];
       inst->Hex[4] = hexTable[inst->rd];

   }
   else if(inst->instType==1)//I Type
   {

       if(inst->op==11)
       {
          inst->Hex[3] = hexTable[inst->rs];
          inst->Hex[2] = hexTable[inst->rt];
       }
       else
       {
          inst->Hex[2] = hexTable[inst->rs];
          inst->Hex[3] = hexTable[inst->rt];
       }

       char Offset[8]={'\0'};

       decimalToHexa(inst->imm , Offset);

       int length = strlen(Offset);

       if(length>=4)
       {
           c=4;
       }
       else
           c=8-length;

       if( inst->imm >= 0 )
       for(int i=length-1; i>=0; i--)
       {
           inst->Hex[c] = Offset[i];
           c++;
       }
       else
       for(int i=3; i>=0; i--)
       {
           inst->Hex[c] = Offset[i];
           c++;
       }

   }
   else if(inst->instType==2)//J Type
   {
       //j
      if(inst->op==13)
      {
          char Offset[5]={'0'};

          decimalToHexa(inst->imm , Offset);

          int length = strlen(&Offset);

          if(length>=4)
          {
            c=4;
          }
          else
          c=8-length;

         if(inst->imm>=0)
         for(int i=length-1; i>=0; i--)
         {
           inst->Hex[c] = Offset[i];
           c++;
         }
         else
         for(int i=3; i>=0; i--)
         {
           inst->Hex[c] = Offset[i];
           c++;
         }
      }
      //halt
      //...
   }




}
int findSymbolTableLen(FILE *inputFile)
{
    int count=0;
    size_t lineSize = 100;
    char *line;
    line = (char *) malloc(72);
    while(fgets(line , lineSize , inputFile) != NULL)
    {
        if((line[0]!=' ') && (line[0]!='\t'))
        {
            count++;
        }
    }

    rewind(inputFile);
    free(line);
    return count;
}
int fillSymTab(struct symbolTable *symbols , FILE * inputFile , FILE *output )
{
    int lineNumber = 0;
    size_t lineSize=100;
    char *line;
    int i=0;
    char *token;
    line = (char *) malloc(72);
    while(fgets(line , lineSize , inputFile) != NULL)
    {
        if((line[0]!=' ' && line[0]!='\t'))
        {
            token = strtok(line , "\t ," );
              if(IsNumber(token))
              {
                  fprintf(output , "%s\n" , "There is an invalid label.");
                  printf("There is an invalid label.\n");
                  exit(1);
              }
              else
              {
                  strcpy(symbols[i].symbol , token);
                  symbols[i].value = lineNumber;
                  i++;
              }
        }
        lineNumber++;
    }

    rewind(inputFile);
    free(line);
    return lineNumber;
}
int FindImmValue(char *offset , struct symbolTable *SymTab , int symbolTableLen , FILE *Output)
{
    int imm;
    for(int i=0; i<symbolTableLen; i++)
    {
        if( (strcmp(SymTab[i].symbol , offset) )==0)
        {
            return SymTab[i].value;
        }
    }

    if(IsNumber(offset))
        return atoi(offset);
    else
    {
        fprintf(Output , "%s\n" , "Error : There is an undefined label.");
        printf("%s\n" , "Error : There is an undefined label.\n");
        exit(1);
    }

}
void check_labels(int length , struct symbolTable *symbols , FILE *output )
{
    int i , j;
    for(i=0; i<length; i++)
    {
        for(j=i+1; j<length; j++)
        {
            if( strcmp(symbols[i].symbol , symbols[j].symbol)==0 )
                if(symbols[i].value != symbols[j].value)
                {
                    fprintf(output , "%s\n" , "Error : The Label has been defined more than once.");
                    printf("Error : The Label has been defined more than once.\n");
                    exit(1);
                }

        }
    }
}
int IsNumber(char *offset)
{
    int j;
    j = strlen(offset);
    if(offset[0]=='-')
        return 1;
    while(j--)
    {
        if(offset[j] > 47 && offset[j] < 58)
            continue;

        return 0;
    }
    return 1;
}
void Print_Binary(struct instruction *inst)
{
    long long int dec = inst->Instdecimal;
    if(dec<0)
    {
        dec+=4294967296;
    }

    for(int i=0; i<32; i++)
    {
        inst->Binary[i] = dec%2;
        dec/=2;

    }
    for(int i=31; i>-1; i--)
    {
        printf("%d" , inst->Binary[i]);
        if(i%4==0 && i!=0)
            printf("-");
    }
    printf("\n");
}
