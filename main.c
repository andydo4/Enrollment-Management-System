#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
#include "leak_detector_c.h"

#define MAXFILE 31
#define MAXSTRING 50

// PHASE 1: search_students, search_course, count_students

// STRUCTURES
typedef struct Student{
  int stuID;
  char *Fname;
  char *Lname;
  int birthday;
  int month;
  int year;
}Student;

typedef struct Enrollment{
  char *eID;
  int sID;
  char *semester;
  float score;
}Enrollment;

typedef struct Course{
  char *cID;
  char *courseName;
  float totalCredit; // use %.0f to print credit
}Course;


///////////////////////////////////

Student* loadStudents(int *numStudents, char studentfile[]){
  FILE *fpStu; // creates a file pointer
  int total = 0; // initializes total of students to zero
  fpStu = fopen(studentfile, "r"); // opens student file 
  if(fpStu == NULL){ // checks if error
    printf("Error opening student file\n");
    exit(1);
  }
  fscanf(fpStu, "%d", &total); // scans first int and sets to total
  Student *students = (Student *)malloc(total * sizeof(Student)); // sets dynamic memory allocation for pointer array students
  if (students == NULL){ // checks if error 
    printf("Memory allocation failed\n");
    exit(1);
  }
  for(int i = 0; i < total; i++){ // loops for amount of stuents 
    students[i].Fname = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
    students[i].Lname = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
    if (students[i].Fname == NULL || students[i].Lname == NULL){ // checks for error 
      printf("Memory allocation failed\n");
      exit(1);
    }
    fscanf(fpStu, "%d %s %s %d %d %d", &students[i].stuID, students[i].Fname, students[i].Lname, &students[i].birthday, &students[i].month, &students[i].year); // scans 
  }
  fclose(fpStu); // closes it
  *numStudents = total;// sets pointer int to total to be sent back to main 
  return students; // returns the newly updated array students 
}

///////////////////////////////////////////

Enrollment* loadEnrollment(int *numEnrolls, char enrollfile[]){
  FILE *fpEnroll; // creates file pointer
  int total = 0; // sets total to 0
  fpEnroll = fopen(enrollfile, "r"); // opens file
  if(fpEnroll == NULL){ // checks for error 
    printf("Error opening enrollment file\n");
    exit(1);
  }
  fscanf(fpEnroll, "%d", &total); // sets total to first int in file
  Enrollment *enrolls = (Enrollment *)malloc(total * sizeof(Enrollment)); // DMA for pointer array enrolls
  if (enrolls == NULL){ //checks for error 
    printf("Memory allocation failed\n");
    exit(1);
  }
  for(int i = 0; i < total; i++){ // loops for each enrollment
    enrolls[i].eID = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
    enrolls[i].semester = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
    if (enrolls[i].eID == NULL || enrolls[i].semester == NULL){ // checks for error 
      printf("Memory allocation failed\n");
      exit(1);
    }
    fscanf(fpEnroll, "%s %d %s %f", enrolls[i].eID, &enrolls[i].sID, enrolls[i].semester, &enrolls[i].score); // scans
  }
  fclose(fpEnroll); // closes file
  *numEnrolls = total; // sets total
  return enrolls; // returns newly updated array 
}
/////////////////////////////////////////

Course* loadCourses(int *numCourses, char coursefile[]){
  FILE *fpC; // file pointer
  int total = 0; // sets total to zero
  fpC = fopen(coursefile, "r"); // opens file
  if(fpC == NULL){ // checks if error
    printf("Error opening course file\n");
    exit(1);
  }
  fscanf(fpC, "%d", &total); // scans total
  Course *courses = (Course *)malloc(total * sizeof(Course)); // DMA 
  if (courses == NULL){ // check if error
    printf("Memory allocation failed\n");
    exit(1);
  }
  for(int i = 0; i < total; i++){ // loops for every course
    courses[i].cID = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
    courses[i].courseName = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
    if(courses[i].cID == NULL || courses[i].courseName == NULL){ //check if error 
      printf("Memory allocation failed\n");
      exit(1);
    }
    fscanf(fpC, "%s %s %f", courses[i].cID, courses[i].courseName, &courses[i].totalCredit); // scans
  }
  fprintf(fpC, "\n");
  fclose(fpC); // closes
  *numCourses = total; // sets total courses
  return courses; // returns courses array 
}

int main(void) {
  char studentfile[MAXFILE], coursefile[MAXFILE], enrollfile[MAXFILE];
  int numStudents, numEnrolls, numCourses;
  scanf("%s %s %s", studentfile, coursefile, enrollfile); // gets user input for file names
  getchar();
  printf("==================\n");
  Student *students = loadStudents(&numStudents, studentfile); // loads student structure
  Enrollment *enrolls = loadEnrollment(&numEnrolls, enrollfile); // loads enroll structure
  Course *courses = loadCourses(&numCourses, coursefile); // loads course structure
  menu(students, numStudents, courses, numCourses, enrolls, numEnrolls, coursefile);
  free(students);
  free(enrolls);
  free(courses);
  return 0;
}

void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, char coursefile[]){
  char cmd[MAXSTRING];
  char selection[MAXSTRING];
  while(1){ // loops until exit 
    scanf("%s", selection); // scans for user input
    if(strcmp(selection, "search_students") == 0){ // checks if user input same as "search_students"
      search_students(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
    }
    else if(strcmp(selection, "search_course") == 0){
      ArrayOfCourses = loadCourses(&amountCourses, coursefile); // reloads ArrayOfCourses
      search_course(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
    }
    else if(strcmp(selection, "add_course") == 0){
      add_course(ArrayOfStudents, amountStudents, ArrayOfCourses, &amountCourses, ArrayOfEnrollments, amountEnrolls, coursefile);
    }
    else if(strcmp(selection, "count") == 0){
      ArrayOfCourses = loadCourses(&amountCourses, coursefile); // reloads ArrayOfCourses
      count(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
    }
    else if(strcmp(selection, "sort") == 0)
      sort(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
    else if(strcmp(selection, "exit") == 0){
      printf("------------------\n");
      printf("Thank you!\n");
      free(ArrayOfStudents);
      free(ArrayOfCourses);
      free(ArrayOfEnrollments);
      exit(0);
    }
  }
}

///////////////////////////////////////////// DO NOT TOUCH ////////////////////////////////////

void search_students(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls){
  char *sub1 = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
  char *lastname = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
  int id, k1, found = 0; // sets id int, k1 for convenience, found 
  scanf("%s", sub1); // scans string for selection term
  if(strcmp(sub1,"lname") == 0){ // checks if sub1 is lname
    scanf("%s", lastname); // gets last name from string
    printf("------------------\n");
    for(int i = 0; i < amountStudents; i++){ // loops for amount of students
      if(strcmp(lastname, ArrayOfStudents[i].Lname) == 0){ // saves position of student as i
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].stuID, ArrayOfStudents[i].Fname, ArrayOfStudents[i].Lname, ArrayOfStudents[i].month, ArrayOfStudents[i].birthday, ArrayOfStudents[i].year);
        for(int j = 0; j < amountEnrolls; j++){ // compares student ID to ID's in enrollment file, saves as j
          if(ArrayOfStudents[i].stuID == ArrayOfEnrollments[j].sID){
            for(int k = 0; k < amountCourses; k++){
              if(strcmp(ArrayOfEnrollments[j].eID, ArrayOfCourses[k].cID) == 0){ // compares 'j' course code to courses, saves as k
                k1 = k; // k is the position of the course taken by 'i' student
                found = 1;
                break;
              }
            }
            char average = getGrade(ArrayOfEnrollments[j].score); // gets average of 'i' student in 'j' course
            printf("%s %s %s %.0f %c\n", ArrayOfEnrollments[j].eID, ArrayOfCourses[k1].courseName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, average); //'i' student's 'j' class ID, 'i' student's 'k1' coursename, 'i' student's 'j' semester for their 'k1' course, and their 'j' score and average. 
          }
        }
        printf("------------------\n");
      }
    }
    if(!found){
      printf("not found\n"); // if found still 0, name is not on list
      printf("------------------\n");
    }
  }
  else if(strcmp(sub1,"id") == 0){
    scanf("%d", &id); // gets student id number
    printf("------------------\n");
    for(int i = 0; i < amountStudents; i++){
      if(id == ArrayOfStudents[i].stuID){ // gets the 'i' position of the student
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].stuID, ArrayOfStudents[i].Fname, ArrayOfStudents[i].Lname, ArrayOfStudents[i].month, ArrayOfStudents[i].birthday, ArrayOfStudents[i].year);
        for(int j = 0; j < amountEnrolls; j++){ 
          if(ArrayOfStudents[i].stuID == ArrayOfEnrollments[j].sID){ // finds what classes student is in, marks as 'j'
            for(int k = 0; k < amountCourses; k++){
              if(strcmp(ArrayOfEnrollments[j].eID, ArrayOfCourses[k].cID) == 0){ // finds the name of the class
                k1 = k; // marks position as 'k1'
                found = 1;
                break;
              }
            }
            char average = getGrade(ArrayOfEnrollments[j].score);
            printf("%s %s %s %.0f %c\n", ArrayOfEnrollments[j].eID, ArrayOfCourses[k1].courseName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, average); // 'i' student's 'j' course ID, 'k1' courseName, 'j' semester, 'j' score and average
          }
        }
        printf("------------------\n");
        break;
      }
    }
    if(!found){
      printf("not found\n"); // if found still 0, name is not on list
      printf("------------------\n");
    }
  }
  free(sub1);
  free(lastname);
  printf("==================\n");
}

////////////////////////////////////////////// DO NOT TOUCH ///////////////////////////////////////

char getGrade(float score){
  if (score >= 90)
    return 'A';
  else if (score >= 80 && score <= 89.99)
    return 'B';
  else if (score >= 70 && score <= 79.99)
    return 'C';
  else if (score >= 60 && score <= 69.99)
    return 'D';
  else
    return 'F';
}

/////////////////////////////////////////////// DO NOT TOUCH ///////////////////////////////////////////

void search_course(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls){
  char *sub1 = (char *)malloc(MAXSTRING * sizeof(char)); //DMA
  char *cid = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
  char *semester = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
  int k1, found = 0; // k1 for convenience and found 
  scanf("%s", sub1); 
  if(strcmp(sub1, "cid") == 0){ // if user input is "cid" does this 
    scanf("%s", cid); // scans the cid
    printf("------------------\n");
    for(int i = 0; i < amountCourses; i++){
      if(strcmp(cid, ArrayOfCourses[i].cID) == 0){ // if the user inputted cid is the same as the ith element...
        printf("%s %.0f\n", ArrayOfCourses[i].courseName, ArrayOfCourses[i].totalCredit); // prints the info
        found = 1; // marks found as true
        for(int j = 0; j < amountEnrolls; j++){
          if(strcmp(ArrayOfCourses[i].cID, ArrayOfEnrollments[j].eID) == 0){ // checks what students have that course
            for(int k = 0; k < amountStudents; k++){
              if(ArrayOfEnrollments[j].sID == ArrayOfStudents[k].stuID){ //checks the stuID in courses and students
                char average = getGrade(ArrayOfEnrollments[j].score); // gets the avg of the student
                printf("%s %s %s %.0f %c\n", ArrayOfStudents[k].Fname, ArrayOfStudents[k].Lname, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, average); // prints the student's info
              }
            }
          }
        }
        printf("------------------\n");
      }
    }
    if(!found){
      printf("not found\n"); // if found still 0, name is not on list
      printf("------------------\n");
    }
  }
  if(strcmp(sub1, "cid_semester") == 0){ // if user input is cid_semester, does this
    scanf("%s %s", cid, semester); // gets cid and semester
    printf("------------------\n");
    for(int i = 0; i < amountCourses; i++){
      if(strcmp(cid, ArrayOfCourses[i].cID) == 0){ // finds course id then prints its name and credit
        printf("%s %.0f\n", ArrayOfCourses[i].courseName, ArrayOfCourses[i].totalCredit);
        found = 1; // marks found as true
        for(int j = 0; j < amountEnrolls; j++){
          if(strcmp(ArrayOfCourses[i].cID, ArrayOfEnrollments[j].eID) == 0 && strcmp(semester, ArrayOfEnrollments[j].semester) == 0){ // finds what students take that course
            for(int k = 0; k < amountStudents; k++){
              if(ArrayOfEnrollments[j].sID == ArrayOfStudents[k].stuID){ // matches the student id
                char average = getGrade(ArrayOfEnrollments[j].score); // gets average
                printf("%s %s %s %.0f %c\n", ArrayOfStudents[k].Fname, ArrayOfStudents[k].Lname, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, average); // prints the students' details
              }
            }
          }
        }
        printf("------------------\n");
      }
    }
    if(!found){
      printf("not found\n"); // if found still 0, name is not on list
      printf("------------------\n");
    }
  }
  free(sub1);
  free(cid);
  free(semester);
  printf("==================\n");
}

//////////////////////////////////// DO NOT TOUCH //////////////////////////////////////////////////////

void add_course(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int *amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, char coursefile[]){
  char *id = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
  char *course = (char *)malloc(MAXSTRING * sizeof(char)); // DMA
  float credit, found = 0; 
  int value;
  scanf("%s %s %f", id, course, &credit);
  printf("------------------\n");
  for(int i = 0; i < *amountCourses; i++){
    if(strcmp(id, ArrayOfCourses[i].cID) == 0){ // checks if inputted id is already in file
      printf("\"course id needs to be unique. This course cannot be added.\"\n");
      printf("------------------\n");
      printf("==================\n");
      return;
    }
  }
  FILE *fp; // if gets to here, that means id is not on file
  fp = fopen(coursefile, "r+"); // creates file pointer and opens coursefile to write in
  fscanf(fp, "%d", &value); // takes the first value as amount of courses
  value++; // increments it by 1
  rewind(fp); // goes back to beginning and rewrites new value
  fprintf(fp, "%d", value);
  fseek(fp, 0, SEEK_END); // goes to end of file
  fprintf(fp, "%s %s %.0f\n", id, course, credit); // prints new course
  fclose(fp); // closes file
  printf("record added\n");
  printf("------------------\n");
  printf("==================\n");
  free(id);
  free(course);
}

//////////////////////////////////////////// DO NOT TOUCH ///////////////////////////////////////////////////

void count(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls){
  printf("------------------\n");
  char *option = (char *)malloc(MAXSTRING * sizeof(char));
  scanf("%s", option); // takes user input
  if(strcmp(option, "students") == 0){ // if students, then prints the total amt of students
    printf("total students %d\n", amountStudents);
    printf("------------------\n");
    printf("==================\n");
  }
  if(strcmp(option, "students_course") == 0){
    for (int i = 0; i < amountCourses; i++){ // first we need to check how many students in each course [i]
      int students = 0;
      for (int j = 0; j < amountEnrolls; j++){
        if(strcmp(ArrayOfCourses[i].cID, ArrayOfEnrollments[j].eID) == 0){ // if course id matches student id...
          students++; // increments by 1
        }
      }
      printf("%s %d\n", ArrayOfCourses[i].cID, students); // prints final total
    }
    printf("------------------\n");
    printf("==================\n");
  }
  if(strcmp(option, "students_semester") == 0){ 
    char semList[MAXFILE][MAXSTRING];
    char temp[MAXSTRING];
    int semCount[MAXFILE] = {0};
    int position = 0;
    strcpy(semList[position], ArrayOfEnrollments[0].semester); // setting a default semester to compare to
    strcpy(temp, ArrayOfEnrollments[0].semester); // makes a copy of base semester
    position++; // moves to next position
    for(int i = 0; i < amountEnrolls; i++){
      if(strcmp(temp, ArrayOfEnrollments[i].semester) != 0){ // if base semester is different
        strcpy(semList[position++], ArrayOfEnrollments[i].semester); // copies that semester to the semList
        strcpy(temp, ArrayOfEnrollments[i].semester); // temp becomes that semester
      }
    }
    for(int j = 0; j < position; j++){ 
      for(int k = 0; k < amountEnrolls; k++){
        if(strcmp(semList[j], ArrayOfEnrollments[k].semester) == 0){ // checks how many students in that semester
          semCount[j]++;
        }
      }
      printf("%s %d\n", semList[j], semCount[j]); // prints the semester and its students count
    }
    printf("------------------\n");
    printf("==================\n");
    free(option);
  }
}

//////////////////////////////////////////// DO NOT TOUCH ///////////////////////////////////////////////////

void sort(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls){
  int *id_array = (int *)malloc(amountStudents * sizeof(int));
  int temp;
  printf("------------------\n");
  for(int i = 0; i < amountStudents; i++){
    id_array[i] = ArrayOfStudents[i].stuID; // puts all student ids into id_array
  }
  for(int j = 0; j < amountStudents; j++){
    for(int k = 0; k < amountStudents - j - 1; k++){
      if(id_array[k] > id_array[k+1]){ // sorts the ids in order
        temp = id_array[k];
        id_array[k] = id_array[k+1];
        id_array[k+1] = temp;
      }
    }
  }
  for(int l = 0; l < amountStudents; l++){
    for(int a = 0; a < amountStudents; a++){
      if(id_array[l] == ArrayOfStudents[a].stuID){ // using new ordered ID array, finds same ID in students file
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[a].stuID, ArrayOfStudents[a].Fname, ArrayOfStudents[a].Lname, ArrayOfStudents[a].month, ArrayOfStudents[a].birthday, ArrayOfStudents[a].year); // prints their info
        break;
      }
    }
  }
  free(id_array);
  printf("------------------\n");
  printf("==================\n");
}