#include <Arduino.h>

#include "MMouse.h"


class cell {
  public:
    bool wallN;
    bool wallS;
    bool wallW;
    bool wallE;
    bool hasUpdated;
    bool hasTraveled;
    bool isCenter;
    bool currentlyOn;
    cell() {
      wallN = false;
      wallS = false;
      wallW = false;
      wallE = false;
      hasUpdated = false;
      hasTraveled = false;
      isCenter = false;
      dToC = -1;
    }
    int dToC;
};
bool filledBoard();
void decisions();
void remapBoard();
void clearBoard();
void turnRight();
void turnLeft();
void straight();
void back();
void straightInitialize();
bool rightWallFollow();

#define MOTOR_STEPS 200
#define DIR_X 11
#define STEP_X 10 
#define DIR_Y 8
#define STEP_Y 9
#define _STEPSINREV 200
#define ENABLE 12
#define M0 A5
#define M1 A4

byte microsteps = 8;
DRV8834    left(_STEPSINREV, 11, 10, ENABLE, M0, M1);
DRV8834    right(_STEPSINREV, 8, 9, ENABLE, M0, M1);
// Pick one of the two controllers below
// each motor moves independently, trajectory is a hockey stick
SyncDriver controllerM(right, left);

MMouse mousea(controllerM);

cell board[10][10];
int x = 0;
int y = 0;
int cDir;// 0 = n, 1 = w, 2 = e, 3 = s

bool filledBoard() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (board[i][j].dToC == -1) {
        return false;
      }
    }
  }
  return true;
}


void remapBoard() {
  int n = 0;
  bool left = mousea.isWallLeft();
  bool right = mousea.isWallRight();
  bool front = mousea.isWallFront();

  if (left && cDir == 0) {
    board[x][y].wallW = true;
  }
  else if (left && cDir == 1) {
    board[x][y].wallS = true;
  }
  else if (left && cDir == 2) {
    board[x][y].wallN = true;
  }
  else if (left && cDir == 3) {
    board[x][y].wallE = true;
  }


  else if (right && cDir == 0) {
    board[x][y].wallE = true;
  }
  else if (right && cDir == 1) {
    board[x][y].wallN = true;
  }
  else if (right && cDir == 2) {
    board[x][y].wallS = true;
  }
  else if (right && cDir == 3) {
    board[x][y].wallW = true;
  }


  else if (front && cDir == 0) {
    board[x][y].wallN = true;
  }
  else if (front && cDir == 1) {
    board[x][y].wallW = true;
  }
  else if (front && cDir == 2) {
    board[x][y].wallE = true;
  }
  else if (front && cDir == 3) {
    board[x][y].wallS = true;
  }

  while (!filledBoard) {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        if (board[i][j].dToC == n) {
          if (!board[i][j].wallN && board[i + 1][j].dToC < n) {       //Possible mistake of i + 1 or j + 1   and versus or
            board[i + 1][j].dToC = n + 1;
          }
          else if (!board[i][j].wallS && board[i - 1][j].dToC < n && i > 0) {       //Possible mistake of i + 1 or j + 1   and versus or
            board[i - 1][j].dToC = n + 1;
          }
          else if (!board[i][j].wallW && board[i][j - 1].dToC < n  && j > 0) {       //Possible mistake of i + 1 or j + 1   and versus or
            board[i][j - 1].dToC = n + 1;
          }
          else if (!board[i][j].wallE && board[i][j + 1].dToC < n) {       //Possible mistake of i + 1 or j + 1   and versus or
            board[i][j + 1].dToC = n + 1;
          }
        }

      }
    }
    n++;
    if (n >= 96) {
      break;
    }
  }
}

void clearBoard() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      board[i][j].dToC == 0;
    }
  }
}

void turnRight() {
  mousea.rotate(360); //turning in degrees

}

void turnLeft() {
  mousea.rotate(360);//turning in degrees, will be - of right
}

void straight() {
  mousea.goStraight(10);//distance in mm
}
void back() {
  mousea.goStraight(-10);//distance in mm
}
void straightInitialize() {
  mousea.goStraight(-100);//distance in mm
}
bool rightWallFollow () {
  controllerM.startMove(1000, 1000);

  while (!mousea.isWallFront()) {
    for (int i = 0; i < 2000; i++) 
      controllerM.nextAction();
 
    mousea.testIRSensors();

    if (mousea.IRsensorsValues[2] < 530) {
      controllerM.startMove(1000, 2000);
    }
    else if (mousea.IRsensorsValues[2] > 570) {
      controllerM.startMove(2000,1000);
    } 
    else {
      controllerM.startMove(1000,1000);
    }
  }
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!");
  return true;
}
void setup() {
    Serial.begin(115200);
    delay(10000);
Serial.print("pls work");
digitalWrite(LED_BUILTIN, HIGH);

//************* Cell Setup *****************
  board[4][4].dToC = 0;
  board[4][5].dToC = 0;
  board[5][4].dToC = 0;
  board[5][5].dToC = 0;

  for (int i = 0; i < 10; i++) {
    board[i][0].wallS = true;
    board[0][i].wallW = true;
    board[i][10].wallN = true;
    board[10][i].wallE = true;
  }

//*********** Mouse Setup ****************
  mousea.setupMMouse();
  left.begin(60, 1);
  right.begin(60, 1);
  controllerM.enable();
  left.setSpeedProfile(left.LINEAR_SPEED, 2000, 1000);
  right.setSpeedProfile(right.LINEAR_SPEED, 2000, 1000);
  
//************* 1st Move ******************
  //straightInitialize();
  


}
void decisions() {

  //test if center you are at center
  if (board[x][y].dToC == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10000000);
    }

    // test different directions, then test if distance to center of turning block is shorter
  if (cDir == 0 && 
  (board[x + 1][y].dToC < board[x-1][y].dToC) && 
  (board[x + 1][y].dToC < board[x][y+1].dToC) && 
  (board[x + 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallE) {
    turnRight();
    if (x < 9) {x++;}
  }
  
  else if (cDir == 0 && 
  (board[x - 1][y].dToC < board[x+1][y].dToC) && 
  (board[x - 1][y].dToC < board[x][y+1].dToC) && 
  (board[x - 1][y].dToC < board[x][y-1].dToC) && 
  !board[x][y].wallW) {
    turnLeft();
    if (x < 9) {x--;}
  }
  
  else if (cDir == 0 && (board[x][y+1].dToC < board[x-1][y].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && !board[x][y].wallN) {
    straight();
    if (x < 9) {y++;}
  }
  
  else if (cDir == 0 && (board[x][y-1].dToC > board[x-1][y].dToC) && (board[x][y-1].dToC > board[x][y+1].dToC) && (board[x][y-1].dToC > board[x+1][y].dToC) && !board[x][y].wallS) {
    back();
    if (x < 9) {y--;}
  }




  else if (cDir == 1 && (board[x + 1][y].dToC < board[x-1][y].dToC) && (board[x + 1][y].dToC < board[x][y+1].dToC) && (board[x + 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallE) {
    back();
    if (x < 9) {x++;}
  }
  
  else if (cDir == 1 && (board[x - 1][y].dToC < board[x+1][y].dToC) && (board[x - 1][y].dToC < board[x][y+1].dToC) && (board[x - 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallW) {
    straight();
    if (x < 9) {x--;}
  }
  
  else if (cDir == 1 && (board[x][y+1].dToC < board[x-1][y].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && !board[x][y].wallN) {
    turnRight();
    if (x < 9) {y++;}
  }
  
  else if (cDir == 1 && (board[x][y-1].dToC > board[x-1][y].dToC) && (board[x][y-1].dToC > board[x][y+1].dToC) && (board[x][y-1].dToC > board[x+1][y].dToC) && !board[x][y].wallS) {
    turnLeft();
    if (x < 9) {y--;}
  }




   else if (cDir == 2 && (board[x + 1][y].dToC < board[x-1][y].dToC) && (board[x + 1][y].dToC < board[x][y+1].dToC) && (board[x + 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallE) {
    straight();
    if (x < 9) {x++;}
  }
  
  else if (cDir == 2 && (board[x - 1][y].dToC < board[x+1][y].dToC) && (board[x - 1][y].dToC < board[x][y+1].dToC) && (board[x - 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallW) {
    back();
    if (x < 9) {x--;}
  }
  
  else if (cDir == 2 && (board[x][y+1].dToC < board[x-1][y].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && !board[x][y].wallN) {
    turnLeft();
    if (x < 9) {y++;}
  }
  
  else if (cDir == 2 && (board[x][y-1].dToC > board[x-1][y].dToC) && (board[x][y-1].dToC > board[x][y+1].dToC) && (board[x][y-1].dToC > board[x+1][y].dToC) && !board[x][y].wallS) {
    turnRight();
    if (x < 9) {y--;}
  }



   else if (cDir == 3 && (board[x + 1][y].dToC < board[x-1][y].dToC) && (board[x + 1][y].dToC < board[x][y+1].dToC) && (board[x + 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallE) {
    turnLeft();
    if (x < 9) {x++;}
  }
  
  else if (cDir == 3 && (board[x - 1][y].dToC < board[x+1][y].dToC) && (board[x - 1][y].dToC < board[x][y+1].dToC) && (board[x - 1][y].dToC < board[x][y-1].dToC) && !board[x][y].wallW) {
    turnRight();
    if (x < 9) {x--;}
  }
  
  else if (cDir == 3 && (board[x][y+1].dToC < board[x-1][y].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && (board[x][y+1].dToC < board[x][y-1].dToC) && !board[x][y].wallN) {
    back();
    if (x < 9) {y++;}
  }
  
  else if (cDir == 3 && (board[x][y-1].dToC > board[x-1][y].dToC) && (board[x][y-1].dToC > board[x][y+1].dToC) && (board[x][y-1].dToC > board[x+1][y].dToC) && !board[x][y].wallS) {
    straight();
    if (x < 9) {y--;}
  }
  
}
void loop() {
  //decisions();
  /*for (int i = 0; i < 10; i++) {
  mousea.rotate(360);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW );
  
}*/
delay(10000);
}
