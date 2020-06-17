/*
    Kevin Ramos
    COP 3502
    Program 1: CourseSummary.c
*/

/*
    This function takes the information pertaining to a number of courses and outputs the following information for each:
    course name, how many students passed, a list of section averages, and the details of the student with the highest avg_score.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLENGTH 20
#define MAXSTUDENTS 500
#define MAXASSIGNMENTS 20

// Required Structures typedef student and course
typedef struct student{
    int id;
    char *lname; //stores last name of student
    float *scores; //stores scores of the student. Size is taken from num_scores array.
    float std_avg; //average score of the student (to be calculated)
}student;

typedef struct course{
    char *course_name; //stores course name
    int num_sections; //number of sections
    student **sections; //stores array of student arrays(2D array). Size is num_sections;
    int *num_students;//stores array of number of students in each section. Size is num_sections;
    int *num_scores; //stores array of number of assignments in each section. Size is num_sections;
}course;

// Required functions
course* read_courses(int *num_courses);
student** read_sections(int num_students[], int num_scores[], int num_section);
void process_courses(course *courses, int num_courses);
void release_courses(course *courses, int num_counrses);

// Main function
int main(){
    
    int t = 0, c = 0;
    int i = 0;
    int num_courses = 0;
    
    // Prompt the user for the number of test cases
    scanf("%d", &t);
    
    // Loop for each test case
    while (i < t)
    {
        printf("test case: %d\n", i+1);
        scanf("%d", &c);
        
        // Set num_courses to c
        num_courses = c;
        
        // Call read_courses to create an array of courses
        course* courses = read_courses(&num_courses);
               
        // Process each course, display the information
        process_courses(courses, num_courses);
        
        // Free the memory for the test case
        release_courses(courses, num_courses);
        
        i++;
    }
}

// Pre-condition:   Takes in num_courses (how many courses the test case has) for each test case.
// Post-condition:  Returns a pointer to an array of courses with specific information.
course* read_courses(int *num_courses)
{
    int i = 0;
    char tempname[MAXLENGTH+1];
    
    // course* temp is an array of courses.
    course* temp = (course *) malloc (*num_courses * sizeof(course));
    
    // For each course, allocate space for the sections and define their variables
    for(i = 0; i < *num_courses; i++)
    {
        
        // Read the name and update
        scanf("%s", tempname);
        
        int namelength = strlen(tempname);
        
        // Allocate space for the course name.
        temp[i].course_name = (char *) malloc(sizeof(char)*namelength+1);
        
        strcpy(temp[i].course_name, tempname);
        
        // Read number of sections
        scanf("%d", &temp[i].num_sections);

        // Allocate space for the sections
        temp[i].sections = (student **) malloc(sizeof(student *) * temp[i].num_sections);
        
        // Allocate space for the num_students and num_scores arrays, their size is num_sections.
        temp[i].num_students = (int *) malloc (sizeof(int) * temp[i].num_sections);
        temp[i].num_scores = (int *) malloc (sizeof(int) * temp[i].num_sections);
        
        // Call read_sections to create students 2D array
        temp[i].sections = read_sections(temp[i].num_students, temp[i].num_scores, temp[i].num_sections);
    }
    
    // Return the array of courses to main
    return temp;
}


// Pre-condition:   Take the reference of a course's number of students and assignments per section
// Post-condition:  Reads the data for all sections and returns a 2D array of student information
student** read_sections(int num_students[], int num_scores[], int num_sections)
{
    char tempname[20];
    int tempid = 0;
    float tempscore = 0.0;
    float total = 0.0;
    
    // tempStudents is an array of student pointers. Allocate memory for array of student pointers
    // Each student pointer represents a section.
    student** tempStudents = (student **) malloc (sizeof(student*) * num_sections);
    
    for(int i = 0; i < num_sections; i++)
    {
        int nStudents = 0, nScores = 0;
        
        // Read in number of students and scores per section
        scanf("%d %d", &nStudents, &nScores);

        num_students[i] = nStudents;
        
        // TempStudents[i] is a student pointer, an array of student structs
        // Allocate memory for the array of student structs
        tempStudents[i] = (student *) malloc (sizeof(student) * nStudents);
        
        // Allocate required memory for each student struct
        for(int j = 0; j < nStudents; j++)
        {
            // Prompt the user for each student's ID and name
            scanf("%d", &tempid);
            tempStudents[i][j].id = tempid;
            scanf("%s", tempname);
            
            // Allocate memory for the student's name and copy it
            tempStudents[i][j].lname = (char *) malloc(sizeof(char)*(strlen(tempname)+1));
            strcpy(tempStudents[i][j].lname, tempname);
            
            // Allocate memory for the student's scores
            tempStudents[i][j].scores = (float *) malloc(sizeof(float) * nScores);
            
            // Read in each student's scores and store it
            for(int k = 0; k < nScores; k++)
            {
                scanf("%f", &tempscore);
                tempStudents[i][j].scores[k] = tempscore;
                total += tempStudents[i][j].scores[k];
            }
            
            // Calculate the student's average score
            tempStudents[i][j].std_avg = total / nScores;
            total = 0.0;

        }
    }
    
    // Returns 2D students array with id, lname, and scores to read_courses
    return tempStudents;
    
}

// Pre-condition:   Takes the the information of a number of courses including section and student information
// Post-condition:  Processes each course to determine how many students passed, display the section averages, and state the information of the student with the best average.
void process_courses(course *courses, int num_courses)
{
    for(int i = 0; i < num_courses; i++)
    {
        int pass_count = 0;
        float sectscore = 0;
        float best_avg = 0;
        int best_stud_index = 0; // used to keep track of best student in section
        int best_sect_index = 0; // used to keep track of index that best student resides
        
        // Print course name
        printf("%s ", courses[i].course_name);
    
        float sect_avgs[courses[i].num_sections];
        
        // Process info for each section and the course overall
        for(int j = 0; j < courses[i].num_sections; j++)
        {
            sectscore = 0;

            for(int k = 0; k < courses[i].num_students[j]; k++)
            {
                // Keep track of the student with the best average
                if(courses[i].sections[j][k].std_avg > best_avg)
                {
                    best_avg = courses[i].sections[j][k].std_avg;
                    best_stud_index = k;
                    best_sect_index = j;
                }
                
                // Check if student in the section passed
                if(courses[i].sections[j][k].std_avg >= 70.0)
                {
                    pass_count++;
                }
                
                // Add up the section scores
                sectscore += courses[i].sections[j][k].std_avg;
            }
            
            // Calculate the section average
            sect_avgs[j] = sectscore / (courses[i].num_students[j]);
        }
        
        // Print the number of students who passed the course.
        printf("%d ", pass_count);
        
        // Print the section averages.
        for(int l = 0; l < courses[i].num_sections; l++)
        {
            printf("%.2f ", sect_avgs[l]);
        }
        
        // Display the student with the best average in the course.
        
        printf("%d %s %.2f\n", courses[i].sections[best_sect_index][best_stud_index].id,courses[i].sections[best_sect_index][best_stud_index].lname, courses[i].sections[best_sect_index][best_stud_index].std_avg);
    }
}

// Pre-condition:   Takes in the number of courses and the courses array for the test case.
// Post-condition:  Frees all dynamically allocated memory for the course and student structures.
void release_courses(course *courses, int num_courses)
{
    for(int i = 0; i < num_courses; i++)
    {
        for(int j = 0; j < courses[i].num_sections; j++)
        {
            for(int k = 0; k < courses[i].num_students[j]; k++)
            {
                free(courses[i].sections[j][k].lname);
            }
        }
        free(courses[i].course_name);
        free(courses[i].num_students);
        free(courses[i].num_scores);
    }
}
