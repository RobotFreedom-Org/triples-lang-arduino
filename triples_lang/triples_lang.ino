/*
 Arduino Triples-Lang
 RobotFreedom.org 2026
 Lisenses MIT
  
*/

#include "struct.h" 
  
const int MAX_CMD_LINES = 30;
const int MAX_VAR_NAME  = 6; 
const int MAX_BLOCKS    = 2;
const int MAX_CMDS      = 4;  
cmdsSVO BLOCKS[MAX_BLOCKS][MAX_CMDS];

const int MAX_FUNCTS   = 3;
const int MAX_F_CMDS   = 8; 
char FUNCTSSNAMES[MAX_FUNCTS][MAX_VAR_NAME] ;
cmdsSVO FUNCTS[MAX_FUNCTS][MAX_F_CMDS] ;

const int MAX_MEM     = 10; 
char MEMSTR[MAX_MEM][MAX_CMD_LINES] ;
char MEMSTRNAME[MAX_MEM][MAX_VAR_NAME] ; 

float  MEMFLT[MAX_MEM] ;
char MEMFLTNAME[MAX_MEM][MAX_VAR_NAME] ; 

char current_name[ MAX_VAR_NAME] ; 
 



int current_blocks = -1;
int current_mem    = -1;
int loaded_routine    = -1;

char empty = '                              ';
 

void initBlock(int block) {
  int _max = MAX_CMDS - 1;
  for (int i = 0; i < _max; i++) { 
    BLOCKS[block][i].v[0] =  {0};
    BLOCKS[block][i].s[0] =  {0};
    BLOCKS[block][i].o[0] =  {0};
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println("RF.Triples-Lang");  
  int _max = MAX_BLOCKS - 1;
  for (int i = 0; i < _max; i++) {
    initBlock(i);
  }  
} 

int nextBlock() {

  int _max = MAX_BLOCKS - 1;
  for (int i = 0; i < _max; i++) {
    if (BLOCKS[i][0].s[0] == '\0') {
      return i;
    }
  }
  return 0;
}


int nextCmd(int block) {

  int _max = MAX_CMDS;  
  for (int i = 0; i < _max; i++) { 
    if (BLOCKS[block][i].s[0] == '\0') { 
      return i;
    }
  }
  return 0;
}


int nextFunct() {

  int _max = MAX_FUNCTS - 1;
  for (int i = 0; i < _max; i++) {
    if (FUNCTS[i][0].v[0] == '\0') {
      return i;
    }
  }
  return 0;
}

int getroutineyID(char name[] ) {
 
  int _max = MAX_MEM -1;
  for (int i = 0; i < _max; i++) {  
      if ( strcmp(FUNCTSSNAMES[i], name)  ==0 ) {
        return  i; 
    }
  }
  return  -1;
}  

int getMemoryID(char name[]) {
 
  int _max = MAX_MEM -1;
  for (int i = 0; i < _max; i++) { 
      if  (strcmp(MEMSTRNAME[i], name)  == 0 ) {
 
        return  i; 
    }
  }
  return  -1;
}  

char* getMemory(char name[]) { 
  int _max = MAX_MEM -1; 
 
  for (int i = 0; i < _max; i++) {  
      if (strcmp(MEMSTRNAME[i], name)  ==0 ) {   
         return MEMSTR[i]; 
    }
  }
  return "";
}  

char setMemory(char name[], char value[]  ) { 
  
  int _id = getMemoryID(name);
  if (_id == -1)
  {  
      current_mem ++ ;
      _id = current_mem;
  }  
  int _max = MAX_MEM -1;
  for (int i = 0; i < _max; i++) {  
 
      MEMSTRNAME[_id][i] = name[i];
      MEMSTR[_id][i] = value[i];
  }  
  return "";
}  
 
void copy(char source[], char target[]  ) { 
   
  int _max = strlen(source) ;
  for (int i = 0; i < _max; i++) {     
      target[i] = source[i];  
  }   
 
 
}  

char clearMemory( ) {
 
  int _max = MAX_MEM -1;
  for (int i = 0; i < _max; i++) { 
      MEMSTRNAME[i][0]= {0};
      MEMSTR[i][0] = {0}; 
   }
  current_mem = -1;
  return "";
}  


SVO runIfBlock(int block) {

  int _max = MAX_CMDS - 1;
  int run = 1; 

  for (int i = 0; i < _max; i++) {
    if (BLOCKS[block][i].v != "0" && run == 1) { 
        if (i == 0) {
          
          char*  v =  BLOCKS[block][i].v; 
          char*  s =  BLOCKS[block][i].s;   
          char*  o =  BLOCKS[block][i].o ;   
  
          SVO condition = core(v,s,o); 
          if (condition.o == "0") {
            run = 0; 
 
          }
        } else { 
          char*  v = BLOCKS[block][i].v ;  
          char*  s = BLOCKS[block][i].s;  
          char*  o = BLOCKS[block][i].o;     
          return core(v,s,o);
        } 
    }
  }
}
 

SVO execute( char* o) {
  SVO output;
  int _max = MAX_CMDS - 1;
  int run = 1;
  int block = getroutineyID(o);    
  for (int i = 0; i < _max; i++) {
    if (FUNCTS[block][i].v != "0" && run == 1) { 
          char*  v =  FUNCTS[block][i].v; 
          char*  s =  FUNCTS[block][i].s;   
          char*  o =  FUNCTS[block][i].o ;    
          output = core(v , s, o); 
      }
    } 
  return output;
}


SVO dispatcher(char *v, char *s, char *o) {
 
  SVO output;
  output.v = "";
  output.s = "";
  output.o = "";  
 
 
  if (current_blocks != -1) {
 
     if (strcmp(v, "flow")  == 0  && strcmp(s, "if")  == 0  &&  strcmp(o, "end")  == 0) { 
 
      // Serial.println(current_blocks);
      if (current_blocks <= 0) {  
         output =  runIfBlock(current_blocks);
         BLOCKS[current_blocks][0].s[0] = {0};
         BLOCKS[current_blocks][0].v[0] = {0};
         BLOCKS[current_blocks][0].o[0] = {0};
         current_blocks = current_blocks - 1;  
         return  output;
       }
      else
        { 
         int parent_block = current_blocks - 1; 
         int next_cnd = nextCmd(parent_block);
         
         int _max = MAX_MEM -1;
         for (int i = 0; i < _max; i++) { 
            int j = next_cnd + i;
            copy(BLOCKS[current_blocks][i].v, BLOCKS[parent_block][j].v );
            copy(BLOCKS[current_blocks][i].s, BLOCKS[parent_block][j].s );
            copy(BLOCKS[current_blocks][i].o, BLOCKS[parent_block][j].o ) ;
         }
         BLOCKS[current_blocks][0].s[0] = {0};
         BLOCKS[current_blocks][0].v[0] = {0};
         BLOCKS[current_blocks][0].o[0]  = {0};
         current_blocks = current_blocks - 1; 

         return  output;


        }
    } else if (strcmp(v, "routine")  == 0  && strcmp(s, "create")  == 0  &&  strcmp(o, "end")  == 0) {  

        int cmds_cnt = MAX_CMDS -1;
        int funct_id = nextFunct();

        // Serial.println("creating a routine!");
        // Serial.println(funct_id);
        // Serial.println(current_name);
 
         int _max = MAX_VAR_NAME -1;
         for (int i = 0; i < _max; i++) {   
              FUNCTSSNAMES[funct_id][i] = current_name[i]; 
           }  
  
        for (int i = 0; i <= cmds_cnt; i++) {
          copy(BLOCKS[current_blocks][i].v, FUNCTS[funct_id][i].v );
          copy(BLOCKS[current_blocks][i].s, FUNCTS[funct_id][i].s );
          copy(BLOCKS[current_blocks][i].o, FUNCTS[funct_id][i].o ); 
         } 

         BLOCKS[current_blocks][0].v[0] = {0};
         BLOCKS[current_blocks][0].s[0] = {0};
         BLOCKS[current_blocks][0].o[0] = {0};
         current_blocks = current_blocks - 1; 
         current_name[0]  = {0}; 
         return output;

    } else {
      int current_code = nextCmd(current_blocks);   
      copy( v, BLOCKS[current_blocks][current_code].v );
      copy( o, BLOCKS[current_blocks][current_code].o );
      copy( s, BLOCKS[current_blocks][current_code].s ); 
      return output;
    }

  } else {
    return core(v, s, o);
  }  
}  

/* 
// TODO- short hand to min char size in memeory
// replace all svo that are directives to  pointer in memory when storig routine and flow
 
int (*getOperation(char op))(int, int) {

   if (op == 'add') {
       return &add;  
   } else if (op == 'subract') {
       return &subtract;  
   }
   } else if (op == 'subract') {
       return &subtract;  
   }
   return NULL; // Return NULL if no match
}
int main() { 
   int (*operation)(int, int) = getOperation('+');
   if (operation) {
       printf("Result: %d\n", operation(5, 3));  
   } else {
       printf("Invalid operation\n");
   }
   return 0;
 
//date time funct
  char date[] = "2026,08,05";  
  char *token;   
  token = strtok(date, ",");  
  int year = atoi(token);   
}*/

SVO core(char *v , char *s , char *o ) {
 
  SVO output;
  output.v = "";
  output.s = "";
  output.o = "";   

  if (strcmp(v, "help")   == 0  ) {  
      Serial.println("set,get,echo,add,subtract,divide,multiple,increment,decrement,more,less,equal,different,flow,routine,help");
  } 
  else if (strcmp(v, "annotation") ==0){
      
  }
  else if ( strcmp(v, "echo")   == 0) {
       Serial.println(s); 

  } else if (strcmp(v, "flow")   == 0  ) {
    if ( strcmp(s, "if")   == 0  ) { 
      if (strcmp(o, "start")   == 0  ) {
          current_blocks = nextBlock();  
       }
     }
   }
 
  else if (strcmp(v, "routine")   == 0  ) { 
    if (strcmp(s, "create")   == 0  ) {
 
         current_blocks = nextBlock();   
         current_name[0]= '\0' ; 
         for (int i = 0; i <  strlen(o)  ; i++) {   
             current_name[i] = o[i];
         }    
    }
    else if (strcmp(s, "execute")   == 0  ) {
        output  =  execute(  o);
    }  

  } else if (strcmp(v, "express")   == 0 ) {
    if (strcmp(v, "lights")   == 0) {
      // PLace Holder 
      output.v = "";
    }
  } else if (strcmp(v, "movmeent")   == 0 ) {
    if (strcmp(s, "torso")   == 0 ) {
      // Place Holder
    } else if (strcmp(s, "legs")   == 0) {
      //Place Holder
    }
  } else if (strcmp(v, "set")   == 0 ){ 
  
    char t =  setMemory(s,  o);
    output.o =  o;
 
  } else if (strcmp(v, "get")  ==0){
 
    char* _t = getMemory(s);  
    output.o = String(_t);
    output.o =  _t;

  } else if (v == 'clear') {
    clearMemory();
    output.o = String("cleared");
 
  } else if (strcmp(v, "add")  ==0){
  
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  
    float val = f1 + f2;  
    output.o = String(val);

  } else if (strcmp(v, "increment")  ==0)  {
    char* v1 =  getMemory(s); 
    float f1 =  atof(v1);   
    float val = f1 + 1;
    char t =  setMemory(s,  val);
    output.o = String(val);  

  } else if (strcmp(v, "decrement")  ==0)  {
    char* v1 =  getMemory(s); 
    float f1 =  atof(v1);   
    float val = f1 - 1;
    char t =  setMemory(s,  val);
    output.o = String(val);  
  } else if (strcmp(v, "subtract")  ==0)  {
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  
    float val = f1 - f2;
    output.o = String(val);

  } else if (strcmp(v, "divide")  ==0)   {
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  

    float val = f1 / f2;
    output.o = String(val);

  } else if  (strcmp(v, "multiply")  ==0)  {
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  

    float val = f1 * f2;
    output.o = String(val);

  } else if (strcmp(v, "more")  ==0){
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  

    if (f1 > f2) {
      output.o = "1";
    } else {
       output.o = "0";
    }

  } else if (strcmp(v, "less")  ==0)   {
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  

    if (f1 < f2) {
      output.o = "1";
    } else {
      output.o = "0";
    }

  } else if (strcmp(v, "equal")  ==0)  {
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  

    if (f1 == f2) {
      output.o = "1";
    } else {
      output.o= "0";
    }

  } else if (strcmp(v, "different")  ==0)   {
    char* v1 =  getMemory(s);
    char* v2 =  getMemory(o);  
    float f1 =  atof(v1);  
    float f2 =  atof(v2);  

    if (f1 != f2) {
      output.o = "1";
    } else {
      output.o = "0";
    }
  } 

  return output;
 
}

void replace_char(char *str, char find, char replace) {
    while (*str) {
        if (*str == find) {
            *str = replace;
        }
        str++;
    }
}
 

String inCmds = "";

void loop() {

  char character;
  String t = "";
  while (Serial.available()) {
    character = Serial.read();
    t = character;
    if (t != "\n") {
      inCmds.concat(character);
    }
  }

  if (inCmds == "?") {
    Serial.println("RF.Triples-Lang");
    inCmds = "";
  }
 
  if (inCmds.endsWith(";")) {
 


    char cmds[MAX_CMD_LINES];
   // char *token;   
    char *s;   
    char *v;      
    char *o;   
 
    inCmds.toCharArray(cmds, MAX_CMD_LINES); 
    v = strtok(cmds, " ");  
    s = strtok(NULL, " ");  
    o = strtok(NULL, " ");     
     
    replace_char(o,';', '\0'); 
    inCmds = "";
  
    SVO results = dispatcher(v, s, o);
 
    String final = "";

   // if (results.s != ""  &&  results.v != ""  &&  results.o != "") {
    final.concat(results.s);
    final.concat(" ");
    final.concat(results.v);
    final.concat(" ");
    final.concat(results.o); 
    Serial.println(final);
  //  }
    
  }
}
