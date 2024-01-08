#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

  switch((int)ALUControl){

        //000: Z = A+B
        case 0:
            
          *ALUresult = A + B;
          
          break;
        
        //001: Z = A-B
        case 1:
            
            *ALUresult = A - B;
            
            break;

        //010: If A<B, Z = 1, Otherwise Z = 0
        case 2:
            
            if((signed) A < (signed) B){
                
                *ALUresult = 1;

            }
            else{
                
                *ALUresult = 0;

            }

            break;

        //011: If A<B, Z=1, Otherwise Z = 0
        case 3:
            
            if(A < B){

                *ALUresult = 1;
            }
            else{
                
                *ALUresult = 0;
            }

            break;

        //100: Z = A AND B
        case 4:
            
            *ALUresult = A & B;
            
            break;

        //101: Z = A OR B
        case 5:
            
            *ALUresult = A | B;
            
            break;

        //110: Shift left 16 bits using <<
        case 6:
            
            *ALUresult = B << 16;
            
            break;

        //111: Z = NOT A using ~
        case 7:
            
            *ALUresult = ~A;
            
            break;

    }

    //Assign Zero to 1 if the result is zero; otherwise, assign 0
    if(ALUresult == 0){
        
        *Zero = 1;

    }
    else{
        
        *Zero = 0;

    }
        
}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

  //Used to get proper memory address, (actual location)
  unsigned memAddress = PC >> 2;

  //Pc must be divisible by four, otherwise we throw a hault condition
  if (PC % 4 != 0){

    return 1;

  }
  else {

    * instruction = Mem[memAddress];
    return 0;

  }

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

  //Partition instruction into parts: (op), (r1, r2, r3) (funct, offset, jsec)
  
  // Instructions 5-0
	*funct = instruction & 0b00000000000000000000000000111111;
  // Instructions 15-0
	*offset = instruction & 0b00000000000000001111111111111111;
  // Instructions 25-0
	*jsec = instruction & 0b00000011111111111111111111111111;

	// Instructions 25-21
  *r1 = (instruction >> 21) & 0b11111;
  // Instructions 20-16
	*r2 = (instruction >> 16) & 0b11111;
  // Instructions 15-11
	*r3 = (instruction >> 11) & 0b11111;

  // Instructions 31-26
  *op = (instruction >> 26) & 0b00000000000000000000000000111111;

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    //For MemRead, MemWrite or RegWrite: 1 = enabled, 0 = disabled, 2 = “don’t care”
    switch(op){

        //R-Type below
        case 0:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 7;
            break;

        //J-Type Below

        //Jump
        case 2:
            controls->RegDst = 0;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break; 
        
        //I-Type below

        //beq
        case 4:
            controls->RegDst = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->ALUOp = 1;
            break;
        
        //addi
        case 8:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        
        //slt
        case 10:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 2;
            break;
            
        //sltu
        case 11:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0 ;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 3;
            break;

        //lui
        case 15:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 6;
            break;
            
        //lw
        case 35:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->MemtoReg = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;            
            
        //sw
        case 43:
            controls->RegDst = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->MemtoReg = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        
        
        default:
            return 1;
    }

    return 0;

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

  //Using our values from our register array, populate data 1 and 2
  * data1 = Reg[r1];
  * data2 = Reg[r2];

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

  //Grab the MSB to check the sign
  unsigned negCheck = offset >> 15;

  //If the most significant bit is 1, its negative, so extend with 1s
  //If its not 1, its not negative, so extend with 0s
  if(negCheck == 1)
  {

      * extended_value = offset | 0xFFFF0000;

  }
  else
  {

      * extended_value = offset & 0x0000FFFF;

  }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  
	if(ALUSrc == 1)
  {
		data2 = extended_value;
	}

	if(ALUOp == 7)
  {
		// Check for the correct r-type instruction
		switch(funct) 
    {

			//Add
			case 32:
					ALUOp = 0;
					break;
			//Subtract
			case 34:
					ALUOp = 1;
					break;
			//Set less than signed
			case 42:
					ALUOp = 2;
					break;
			//Set less than unsigned
			case 43:
					ALUOp = 3;
					break;
			//And
			case 36:
					ALUOp = 4;
					break;
			//Or
			case 37:
					ALUOp = 5;
					break;
			//Shift left 
			case 6:
					ALUOp = 6;
					break;

			case 39:
					ALUOp = 7;
					break;
			//Halt
			default:
					return 1;

    }
    // send to ALU function at the end
		ALU(data1,data2,ALUOp,ALUresult,Zero);

	}
  // If ALUOp is not equal to 7 send to ALU function
	else
  {

	ALU(data1,data2,ALUOp,ALUresult,Zero);

	}

	return 0;
  
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
   // read to memory
	if (MemRead == 1) 
  {
		if((ALUresult % 4) == 0)
    {
			*memdata = Mem[ALUresult >> 2];
		}

		//halt
		else
    {
			return 1;
		}

	}

	// write to memory
	if (MemWrite == 1) 
  {
		if((ALUresult % 4) == 0)
    {
			Mem[ALUresult >> 2] = data2;
		}
		//halt
		else
    {
			return 1;
		}
	}

   return 0;
  
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  
  //If we need to write, store our memory to registers and ALU results in r2 and r3
  if(RegWrite == 1)
  {
      if (MemtoReg == 1 && RegDst == 0) 
      {
        Reg[r2] = memdata;
      }
      else if (MemtoReg == 0 && RegDst == 0) 
      {
        Reg[r2] = ALUresult;
      }
      else if(MemtoReg == 1 && RegDst == 1)
      {
        Reg[r3] = memdata;
      }
      else if (MemtoReg == 0 && RegDst == 1)
      {
        Reg[r3] = ALUresult;
      }
	}
    
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
  // Always add 4 to the counter
  *PC = *PC + 4;

  //Jump is multiplied by 4, go to jump register
  if(Jump == 1)
  {
		*PC = (jsec << 2) | (*PC & 0xf0000000);
	}
  
  //If branch returns a zero, bitshift and add the extend
  if(Branch == 1 && Zero == 1)
  {
		*PC += extended_value << 2;
	}

}

