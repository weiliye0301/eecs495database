#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include <limits.h>
#include <iostream>
#include <sstream>
using namespace std;

int current_year;
int current_semester;

int student_semester_decide()

{
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    int x = t_tm->tm_mon+1;
    
    if(x >= 9 && x <= 11)
        
        return 1;
    
    else if (x >= 12 || x <= 2)
        
        return 2;
    
    else if (x >= 3 && x <= 5)
        
        return 3;
    
    else if (x >= 6 && x <= 8)
        
        return 4;
    
    else
        
        return 0;
}
int getSystemTime()
{
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    
    cout<<"Today is "<<t_tm->tm_year+1900<<" "<<t_tm->tm_mon+1<<" "<<t_tm->tm_mday<<endl;
    
    current_year = t_tm->tm_year+1900;
    current_semester = student_semester_decide();
    
    //cout<<"Q" <<current_semester<< endl<< current_year<<endl;
    return 0;
}
int main(int argc, const char * argv[])
{
    getSystemTime();
    char buf[200];
    char course_ID[8];
    char new_password[10];
    char new_address[50];
    char student_ID [11];
    char usr_name_first[10];
    char usr_name_last[10];
    char password[10];
    char selection;
    MYSQL *conn , mysql;

//sprintf(student_ID, "3213");
    
    ////////////////////////LOG_IN/////////////////////////////////////
LOG_IN:
    while(1){
        mysql_init(&mysql);
        conn = mysql_real_connect (
                                   &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                   "zzd", /* password */
                                   "project3-nudb", /* database to use */
                                   3306, /* port (default 3306) */
                                   NULL, /* socket or /var/lib/mysql.sock */
                                   CLIENT_MULTI_RESULTS );
        
        if(conn == NULL)
        {//unable to connect
            printf("Oh Noes!\n");
        }
        else
        {
        }
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int numrows;
        
        
        cout << "Please enter your first name"<< endl;
        
        cin >> usr_name_first;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        
        cout << "Please enter your last name"<< endl;
        
        cin >> usr_name_last;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        
        cout << "Please enter your password"<< endl;
        
        cin >> password;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        sprintf(buf, "select Distinct Password , Id from `project3-nudb`.student where Name = '%s %s';", usr_name_first, usr_name_last);
        
        
        
        mysql_query(&mysql,buf);
        res_set = mysql_store_result(&mysql);
        
        numrows = (int)mysql_num_rows(res_set);
        // Display results
        
        row = mysql_fetch_row( res_set );
        if( numrows != 0 )
        {
            if(strncmp(password, row[0], 10) == 0)
            {
                cout << "Log in successfully" << endl;
                sprintf(student_ID, "%s", row[1]);
                goto STUDENT_MENU;
            }
            else
            {
                cout << "Wrong Password" << endl;
                sprintf(student_ID, "0");
            }
        }
        else
        {
            cout << "Wrong User Name" << endl;
            sprintf(student_ID, "0");
            
        }
        
        
        // free resources
        mysql_free_result(res_set );
        mysql_close(&mysql);}
    
    //////////////////////////////////STUDENT_MENU////////////////////////////////////////////////////
    
STUDENT_MENU:
    while(1){
        mysql_init(&mysql);
        conn = mysql_real_connect (
                                   &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                   "zzd", /* password */
                                   "project3-nudb", /* database to use */
                                   3306, /* port (default 3306) */
                                   NULL, /* socket or /var/lib/mysql.sock */
                                   CLIENT_MULTI_RESULTS );
        
        if(conn == NULL)
        {//unable to connect
            printf("Oh Noes!\n");
        }
        else
        {
        }
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int numrows;
        
        sprintf(buf, "select UoSCode, Semester, Year FROM transcript WHERE Grade is null and StudId = %s and Year = YEAR (CURDATE()) and(( month (CURDATE()) <= 11 and month (CURDATE())>=9 and Semester = 'Q1') OR( (month (CURDATE()) <= 2 or month (CURDATE())>=12) and Semester = 'Q2') OR ( month (CURDATE()) <= 5 and month (CURDATE())>=3 and Semester = 'Q3') OR( month (CURDATE()) <= 8 and month (CURDATE())>=6 and Semester = 'Q4'));", student_ID);
        
        
        
        mysql_query(&mysql,buf);
        res_set = mysql_store_result(&mysql);
        numrows = (int)mysql_num_rows(res_set);
        // Display results
        cout << "---------------------------------"<< endl;
        cout << "|          Student Menu:          |"<< endl;
        cout << "---------------------------------"<< endl;
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row( res_set );
            if( row != NULL )
            {
                cout << "Course: " << row[0] << "  Semester: " << row[1] <<"  Year: " << row[2] << endl;
            }
        }
        
        cout << "Please select"<< endl;
        cout << "1: Transcript"<< endl;
        cout << "2: Enroll"<< endl;
        cout << "3: Withdraw"<< endl;
        cout << "4: Personal Details"<< endl;
        cout << "5: Log Out"<< endl;
        cout << "Any key to refresh"<< endl;
        
        cin >> selection;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        if (selection == '1') {
            selection = '0';
            goto TRANSCRIPT_MENU;
        }
        if (selection == '2') {
            selection = '0';
            goto ENROLLMENT;
        }
        if (selection == '3') {
            selection = '0';
            goto WITHDRAW;
        }
        if (selection == '4') {
            selection = '0';
            goto PERSONAL_DETAILS;
        }
        if (selection == '5') {
            selection = '0';
            //cin.get();
            goto LOG_IN;
        }
        // free resources
        mysql_free_result(res_set );
        mysql_close(&mysql);}
    
    
    
    
    
 //////////////////////////////////////////////////TRANSCRIPT_MENU////////////////////////////////////////////
    
TRANSCRIPT_MENU:
    while(1){
        mysql_init(&mysql);
        conn = mysql_real_connect (
                                   &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                   "zzd", /* password */
                                   "project3-nudb", /* database to use */
                                   3306, /* port (default 3306) */
                                   NULL, /* socket or /var/lib/mysql.sock */
                                   CLIENT_MULTI_RESULTS );

        if(conn == NULL)
        {//unable to connect
            printf("Oh Noes!\n");
        }
        else
        {
        }
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int numrows;
        
        sprintf(buf, "SELECT * FROM transcript where StudId = %s;", student_ID);
        //cout << buf << endl;
        cout << "---------------------------------"<< endl;
        cout << "|          TRANSCRIPT:          |"<< endl;
        cout << "---------------------------------"<< endl;
        
        mysql_query(&mysql,buf);
        res_set = mysql_store_result(&mysql);
        numrows = (int)mysql_num_rows(res_set);
        // Display results
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row( res_set );
            if(row[4]!= NULL)
            {
                cout << "| Course  : " << row[1]<< "| Grade   : " << row[4] << endl;
            }
            else
            {
                cout << "| Course  : " << row[1] << "| Grade   : null" << endl;
            }
        }
        cout << "-----------------------------------------------------------"<<endl;
        
        
        
        // free resources
        selection = '0';
        mysql_free_result(res_set );
        mysql_close(&mysql);
        cout << "Please select one of the following option:" << endl;
        cout << "1: Course Details"<< endl;
        cout << "2: Back to Student Menu"<< endl;
        cout << "Press any key to refresh"<< endl;
        cout << "Your option is : " <<endl;
        cin >> selection;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        if (selection == '1') {
            selection = '0';
            goto COURSE_DETAILS;
        }
        if (selection == '2') {
            selection = '0';
            goto STUDENT_MENU;
        }
    }
    /////////////////////////////////////////////COURSE_DETAILS///////////////////////////////////////////////////////
COURSE_DETAILS:
    while(1){
        mysql_init(&mysql);
        conn = mysql_real_connect (
                                   &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                   "zzd", /* password */
                                   "project3-nudb", /* database to use */
                                   3306, /* port (default 3306) */
                                   NULL, /* socket or /var/lib/mysql.sock */
                                   CLIENT_MULTI_RESULTS );
        
        if(conn == NULL)
        {//unable to connect
            printf("Oh Noes!\n");
        }
        else
        {
        }
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int numrows;
        
        
        cout << "Please enter a course number :"<< endl;
        cin >> course_ID;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        
        sprintf(buf, "SELECT t.UoSCode, UoSName,t.Year, t.Semester, Enrollment, MaxEnrollment,Name,Grade,StudId FROM faculty f, transcript t, unitofstudy un,uosoffering uo where t.UoSCode= '%s' and t.StudId = %s and t.UoSCode=un.UoSCode and t.UoSCode=uo.UoSCode and uo.InstructorId=f.Id and t.Year=uo.Year and t.Semester=uo.Semester;", course_ID,student_ID);
        
        
        mysql_query(&mysql,buf);
        res_set = mysql_store_result(&mysql);
        
        numrows = (int)mysql_num_rows(res_set);
        // Display results
        if(numrows == 0)
        {
            cout << "Wrong course!" << endl;
        }
        else
        {
            
            for (int i = 0; i < numrows; i++) {
                row = mysql_fetch_row( res_set );
                if( row[7] != NULL )
                {
                    
                    cout << "-----------------------------------------------------" << endl;
                    cout << "|               COURSE DETAILS :                    |" << endl;
                    cout << "-----------------------------------------------------" << endl;
                    cout << "| Coursenumber:" << row[0] << endl;
                    cout << "| Title       :" << row[1] << endl;
                    cout << "| Year        :" << row[2] << endl;
                    cout << "| Quarter     :" << row[3] << endl;
                    cout << "| Emrollnumber:" << row[4] << endl;
                    cout << "| MaxEmroll   :" << row[5] << endl;
                    cout << "| Lecturer    :" << row[6] << endl;
                    cout << "| Grade       :" << row[7] << endl;
                    cout << "----------------------------------------------------" << endl;
                }
                else
                {
                    
                    cout << "-----------------------------------------------------" << endl;
                    cout << "|               COURSE DETAILS :                    |" << endl;
                    cout << "-----------------------------------------------------" << endl;
                    cout << "| Coursenumber:" << row[0] << endl;
                    cout << "| Title       :" << row[1] << endl;
                    cout << "| Year        :" << row[2] << endl;
                    cout << "| Quarter     :" << row[3] << endl;
                    cout << "| Emrollnumber:" << row[4] << endl;
                    cout << "| MaxEmroll   :" << row[5] << endl;
                    cout << "| Lecturer    :" << row[6] << endl;
                    cout << "| Grade       :NULL" << endl;
                    cout << "----------------------------------------------------" << endl;
                }
            }

        }
        
        // free resources
        mysql_free_result(res_set );
        
        mysql_close(&mysql);
        
        cout << "Please select one of the following option:" << endl;
        cout << "1: Back to Transcript"<< endl;
        cout << "Press any key to refresh" <<endl;
        cout << "Your option is : " <<endl;
        
        cin >> selection;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        if (selection == '1') {
            selection = '0';
            goto TRANSCRIPT_MENU;
        }
        
        
        
    }
 ////////////////////////////////////////////////ENROLLMENT///////////////////////////////////////////////////////
ENROLLMENT:
    while(1){
        cout << "Please select one of the following option:" << endl;
        cout << "1: Eroll a class"<< endl;
        cout << "2: Back to Student Menu"<< endl;
        cout << "Press any key to refresh" <<endl;
        cout << "Your option is : " <<endl;
        
        cin >> selection;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        if (selection == '2') {
            selection = '0';
            goto STUDENT_MENU;
        }
        if (selection == '1') {
            selection = '0';
            cout << "You can only enroll the class of the current year and quarter or the following quarter." << endl;
            cout << "Please enter the year and quarter you want to enroll class" << endl;
            cout << "Year : " <<endl ;
            int whichyear;
            cin >> whichyear;
            cin.clear();
            cin.ignore( numeric_limits<streamsize>::max(), '\n' );
            cout << "Quarter :"<<endl<<"Q";
            int whichquarter;
            cin >> whichquarter;
            cin.clear();
            cin.ignore( numeric_limits<streamsize>::max(), '\n' );
            
            if ( (whichyear == current_year) && (whichquarter == current_semester) )
            {
                mysql_init(&mysql);
                conn = mysql_real_connect (
                                           &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                           "zzd", /* password */
                                           "project3-nudb", /* database to use */
                                           3306, /* port (default 3306) */
                                           NULL, /* socket or /var/lib/mysql.sock */
                                           CLIENT_MULTI_RESULTS );
                
                if(conn == NULL)
                {//unable to connect
                    printf("Oh Noes!\n");
                }
                else
                {
                }
                MYSQL_RES *res_set;
                MYSQL_ROW row;
                int numrows;
                char class_ID[8];
                sprintf(buf, "SELECT distinct a.UoSCode FROM uosoffering a where a.Year = %d and a.Semester = 'Q%d';", whichyear, whichquarter);
                cout << "---------------------------------"<< endl;
                cout << "|          CLASS AVAILABLE:          |"<< endl;
                cout << "---------------------------------"<< endl;
                
                mysql_query(&mysql,buf);
                res_set = mysql_store_result(&mysql);
                numrows = (int)mysql_num_rows(res_set);
                // Display results
                for (int i = 0; i < numrows; i++) {
                    row = mysql_fetch_row( res_set );
                    if(row!= NULL)
                    {
                        cout << "| Course  : " << row[0]<< endl;
                    }
                    
                }
                cout << "---------------------------------"<<endl;
                cout << "Enter the class ID you want to enroll" << endl;
                cin >> class_ID;
                cin.clear();
                cin.ignore( numeric_limits<streamsize>::max(), '\n' );
                sprintf(buf, "call addclass (%s,'%s', %d, 'Q%d');",student_ID, class_ID, whichyear, whichquarter);
                mysql_free_result(res_set );
                mysql_query(&mysql,buf);
                res_set = mysql_store_result(&mysql);
                numrows = (int)mysql_num_rows(res_set);
                // Display results
                row = mysql_fetch_row( res_set );
                if(row!= NULL)
                {
                    if(!strcmp(row[0], "1"))
                        cout << "Invalid Course!"<< endl;
                    if(!strcmp(row[0], "2"))
                    {
                        cout << "Prerequisite is not met"<< endl;
                        cout << "Prerequits Course  : " << row[1]<< endl;
                    }
                    if(!strcmp(row[0], "3"))
                    {
                        cout << "The capacity of the class is full"<< endl;
                    }
                    if(!strcmp(row[0], "4"))
                    {
                        cout << "You have already enrolled that class"<< endl;
                    }
                    if(!strcmp(row[0], "0"))
                    {
                        cout << "Eroll the class successfully"<< endl;
                    }
                }
                cout << "---------------------------------"<<endl;
                mysql_free_result(res_set );
                mysql_close(&mysql);
            }
            else if((current_semester ==1 && whichyear == current_year +1 && whichquarter == 2)||(current_semester >1 && whichyear == current_year && whichquarter == current_semester + 1))
            {
                mysql_init(&mysql);
                conn = mysql_real_connect (
                                           &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                           "zzd", /* password */
                                           "project3-nudb", /* database to use */
                                           3306, /* port (default 3306) */
                                           NULL, /* socket or /var/lib/mysql.sock */
                                           CLIENT_MULTI_RESULTS );
                
                if(conn == NULL)
                {//unable to connect
                    printf("Oh Noes!\n");
                }
                else
                {
                }
                MYSQL_RES *res_set;
                MYSQL_ROW row;
                int numrows;
                char class_ID[8];
                sprintf(buf, "SELECT distinct a.UoSCode FROM uosoffering a where a.Year = %d and a.Semester = 'Q%d';", whichyear, whichquarter);
                
                cout << "---------------------------------"<< endl;
                cout << "|          CLASS AVAILABLE:          |"<< endl;
                cout << "---------------------------------"<< endl;
                
                mysql_query(&mysql,buf);
                res_set = mysql_store_result(&mysql);
                numrows = (int)mysql_num_rows(res_set);
                // Display results
                for (int i = 0; i < numrows; i++) {
                    row = mysql_fetch_row( res_set );
                    if(row!= NULL)
                    {
                        cout << "| Course  : " << row[0]<< endl;
                    }
                    
                }
                cout << "---------------------------------"<<endl;
                cout << "Enter the class ID you want to enroll" << endl;
                cin >> class_ID;
                cin.clear();
                cin.ignore( numeric_limits<streamsize>::max(), '\n' );
                sprintf(buf, "call addclass (%s,'%s', %d, 'Q%d');",student_ID, class_ID, whichyear, whichquarter);
                mysql_free_result(res_set );
                mysql_query(&mysql,buf);
                res_set = mysql_store_result(&mysql);
                numrows = (int)mysql_num_rows(res_set);
                // Display results
                row = mysql_fetch_row( res_set );
                if(row!= NULL)
                {
                    if(!strcmp(row[0], "1"))
                        cout << "Invalid Course!"<< endl;
                    if(!strcmp(row[0], "2"))
                    {
                        cout << "Prerequisite is not met"<< endl;
                        cout << "Prerequits Course  : " << row[1]<< endl;
                    }
                    if(!strcmp(row[0], "3"))
                    {
                        cout << "The capacity of the class is full"<< endl;
                    }
                    if(!strcmp(row[0], "4"))
                    {
                        cout << "You have already enrolled that class"<< endl;
                    }
                    if(!strcmp(row[0], "0"))
                    {
                        cout << "Eroll the class successfully"<< endl;
                    }
                }
                cout << "---------------------------------"<<endl;
                mysql_free_result(res_set );
                mysql_close(&mysql);
            }
            else
            {
                cout << "Invalid semester selection!"<< endl;
            }
        }
    }
    
 ///////////////////////////////////////WITHDRAW////////////////////////////
    
WITHDRAW:
    while(1){
        
        cout << "Please select one of the following option:" << endl;
        cout << "1: Withdraw a class"<< endl;
        cout << "2: Back to Student Menu"<< endl;
        cout << "Press any key to refresh" <<endl;
        cout << "Your option is : " <<endl;
        
        cin >> selection;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        if (selection == '2') {
            selection = '0';
            goto STUDENT_MENU;
        }
        if (selection == '1')
        {
            selection = '0';
            mysql_init(&mysql);
            conn = mysql_real_connect (
                                       &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                       "zzd", /* password */
                                       "project3-nudb", /* database to use */
                                       3306, /* port (default 3306) */
                                       NULL, /* socket or /var/lib/mysql.sock */
                                       CLIENT_MULTI_RESULTS );
            
            if(conn == NULL)
            {//unable to connect
                printf("Oh Noes!\n");
            }
            else
            {
            }
            MYSQL_RES *res_set;
            MYSQL_ROW row;
            int numrows;
            
            
            cout << "Input the course you want to drop"<< endl;
            cin >> course_ID;
            cin.clear();
            cin.ignore( numeric_limits<streamsize>::max(), '\n' );
            
            sprintf(buf, "call withdraw('%s', %s);",course_ID, student_ID);
            
            mysql_query(&mysql,buf);
            res_set = mysql_store_result(&mysql);
            numrows = (int)mysql_num_rows(res_set);
            // Display results
            row = mysql_fetch_row( res_set );
            if(!strcmp(row[0], "1"))
            {
                cout << "Invalid course name!"<< endl;
            }
            if(!strcmp(row[0], "2"))
            {
                cout << "That course has been graded! You can't drop it!"<< endl;
            }
            if(!strcmp(row[0], "0"))
            {
                cout <<"Drop the course successfully!"<< endl;
                if(!strcmp(row[1], "1"))
                {
                    cout << "Warning Message: Enrollment goes below 50% of capacity!" << endl;
                }
                cout << "-----------------------------------------------------" << endl;
                cout << "|               CURRENT COURSES :                    |" << endl;
                cout << "-----------------------------------------------------" << endl;
                for (int i = 0; i < numrows; i++)
                {
                    row = mysql_fetch_row( res_set );
                    if( row != NULL )
                    {
                        
                        if( row[3] != NULL )
                        {
                            
                            
                            cout << "| Coursenumber:" << row[2] << "| Grade:" << row[3] <<"|Year:" << row[4] << "|Quarter:" << row[5] << endl;
                            
                        }
                        else
                        {
                           cout << "| Coursenumber:" << row[2] << "| Grade: NULL" <<"|Year:" << row[4] << "|Quarter:" << row[5] << endl;
                        }
                    }
                }
                cout << "---------------------------------"<<endl;
            }
            // free resources
            mysql_free_result(res_set );
            
            mysql_close(&mysql);
        }
        
       }
    
  //////////////////////////////////////////////////////////////PERSONAL_DETAILS/////////////////////////////////////
PERSONAL_DETAILS:
    while(1){
        mysql_init(&mysql);
        conn = mysql_real_connect (
                                   &mysql, /* pointer to connection handler */ "localhost", /* host to connect to */ "root", /* user name */
                                   "zzd", /* password */
                                   "project3-nudb", /* database to use */
                                   3306, /* port (default 3306) */
                                   NULL, /* socket or /var/lib/mysql.sock */
                                   CLIENT_MULTI_RESULTS );
        
        if(conn == NULL)
        {//unable to connect
            printf("Oh Noes!\n");
        }
        else
        {
        }
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        int numrows;
        
        
        cout << "-----------------------------------Student Details----------------------------"<< endl;
    
        sprintf(buf, "select * from student where id = %s",student_ID);
        
        
        
        mysql_query(&mysql,buf);
        res_set = mysql_store_result(&mysql);
        
        numrows = (int)mysql_num_rows(res_set);
        // Display results
        for (int i = 0; i < numrows; i++) {
            row = mysql_fetch_row( res_set );
            if( row != NULL )
            {
                cout << "ID : " << row[0] << "  Name: " << row[1] << " Address: "<< row[3]<< endl;
                
            }
        }
        cout << "-----------------------------------Student Details----------------------------"<< endl;
        // free resources
       
        cout << "Please select one of the following option:" << endl;
        cout << "1: Change password"<< endl;
        cout << "2: Change address"<< endl;
        cout << "3: Back to Student Menu"<< endl;
        cout << "Press any key to refresh" <<endl;
        cout << "Your option is : " <<endl;
        
        cin >> selection;
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        if (selection == '3') {
            selection = '0';
            goto STUDENT_MENU;
        }
        else if(selection == '1')
        {
            selection = '0';
            cout << "Input the new password" << endl;
            cin  >> new_password;
            cin.clear();
            cin.ignore( numeric_limits<streamsize>::max(), '\n' );
            sprintf(buf, "update student set Password = '%s' where id = %s",new_password ,student_ID);

            mysql_query(&mysql,buf);
        }
        else if(selection == '2')
        {
            selection = '0';
            cout << "Input the new address" << endl;
            cin  >> new_address;
            cin.clear();
            cin.ignore( numeric_limits<streamsize>::max(), '\n' );
            sprintf(buf, "update student set Address = '%s' where id = %s",new_address ,student_ID);
            mysql_query(&mysql,buf);
        }
        mysql_free_result(res_set );
        
        mysql_close(&mysql);
        
    }
    
    
}