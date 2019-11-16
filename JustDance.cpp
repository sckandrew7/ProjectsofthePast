//created by Kenya Andrews and Eunice
/*the purpose of this program is to store
 movements that can be randomly selected to be given
 to a user specified number of dancers to dance to*/
#include<iostream>
#include <string>

using namespace std;

void isValid (int); //input validation
void setStruct(int);//



struct Movement
{
    //struct members
    string shape;
    string emotion;
    string strength;
    string positioning;
    string level;
    
    
};
void printDances(Movement dances[20], int);

int main()
{
    int numDancers;
    cout<< "WELCOME TO JUST DANCE!!!"<<endl;
    cout << "Please enter how many dancers will be a part of the choreography" <<endl;
    cin >>numDancers;
    isValid(numDancers);
    setStruct(numDancers);
    

    
    return 0;
}

void setStruct(int numDancers)
{
    Movement dances[20] =
    {
        {"Movement: Azonto","Emotion: Happy ","Strength: Medium", "Position: Center","Level: High"},
        
        {"Movement: Akayida","Emotion: Happy ","Strength:Easy ", "Position: Downstage center","Level: High"},
        
        {"Movement: Borrowdale","Emotion: Serious ","Strength: Medium ", "Position:Centre ","Level: High"},
            
        {"Movement: Clarks","Emotion: Happy ", "Strength:low ", "Position: Centre","Level: High"},
        
        {"Movement: pansula leg shift","Emotion: ", "Strength: Medium ", "Position:Center ", "Level: High"},
        
        {"Movement: Shoki","Emotion: Excitied ","Strength:Strong ", "Position Downstage center ","Level: High"},
        
        {"Movement: Skelewu","Emotion: Surprised", "Strength: Medium", "Position:Downstage Center ","Level: High"},
        
        {"Movement: Slowmotion Walk","Emotion: emotionless","Strength:low ", "Position:: Stage Left -> Right","Level: High"},
            
        {"Movement: Magetsi","Emotion: Hype", "Strength: medium  ", "Position:center ","Level: High"},
        
        {"Movement: Bumper","Emotion: happy", "Strength:low ", "Position:Downstage Center", "Level: High -> Mid"},
  
        {"Movement: Rub Shoulder", " Emotion: disgust", " Strength: Easy", " Positioning: Center Stage", " Level: High"},
        
        {"Movement: Reach up and Pull Down - Right Hand", " Emotion: Depression", " Strength: Intense", "  Positioning: Down-Stage Right", " Level: Medium"},
        
        {"Movement: Drop", " Emotion: Hype", " Strength: Easy", " Positioning: Up Stage", " Level: Mid"},
        
        {"Movement: Hit the Quan ", " Emotion: happy ", " Strength: Medium Strength ", " Positioning: Facing Up Stage, placed center stage", " Level: medium"},
        
        {"Movement: Spin", " Emotion: Hype ", " Strength: Easy ", " Positioning: Down Stage", " Level: low"},
        
        {"Movement: Nae Nae", " Emotion: Confused", " Strength: Easy", " Positioning: Center", " Level: mid"},
        
        {"Movement: khumalo", " Emotion: provocotive", " Strength: high", " Positioning: Center", " Level: High"},
        
        {"Movement: whip", " Emotion: Hype", " Strength: strong", " Positioning: facing left stage", " Level: Mid"},
        
        {"Movement: Jaffe Walk ", " Emotion: Angry", " Strength: medium", " Positioning: Stage Left -> Stage Right", " Level: High"},
        
        {"Movement: Dab", " Emotion: Hype", " Strength: Medium", " Positioning: Center Right", " Level: High"}
    };
    
    printDances(dances, numDancers);
}


void printDances(Movement dances[], int numDancers)
{
    for(int j=0; j<numDancers; j++)
    {
        cout<< endl << "__________"<< endl << "Moves for Dancer #: " << j+1 << endl;
        for(int i=0; i<10; i++)
        {
            cout<< endl << "Dance Move #" << i+1 <<endl;
            int randIndex = rand() % 20 + 0;
            cout << dances[randIndex].shape << endl << dances[randIndex].emotion << endl << dances[randIndex].strength << endl << dances[randIndex].positioning << endl << dances[randIndex].level << endl;
        }
        
    }
    
    exit(0);
    
}


void isValid (int numDancers)
{
    
    //input validation for non- integer characters
    if(cin.fail()) //if the user inputs invalid information...
    {
        cout << "Invalid Entry." <<endl; //Invalid statement
        
        while(cin.fail()) //input can only be of integer type
        {
            cin.clear(); //clears out what was previously entered by user
            cin.ignore(); //ignore bad characters inputed by the user
            cout << "Please enter a  valid, new number." <<endl; //ask user to try again
            cin >> numDancers; //get user's next input attempt.
            
        }
        }
    
    // input validation for numbers
    while(numDancers < 1) //int cannot be less than 1
    {
        cout<< "Wrong Input. Input a number 1 or greater" <<endl; //Invalid statement
        cin >> numDancers; //get number of flip times
        
        if(cin.fail()) //if the user inputs invalid information...
        {
            cout << "Invalid Entry." <<endl; //Invalid statement
            
            while(cin.fail()) //input can only be of integer type
            {
                cin.clear(); //clears out what was previously entered by user
                cin.ignore(); //ignore bad characters inputed by the user
                cout << "Please enter a  valid, new number." <<endl; //ask user to try again
                cin >> numDancers; //get user's next input attempt.
            }
        }
        
    }
    setStruct(numDancers);
}

    
    