#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define FILENAME_SIZE 1024
#define MAX_LINE 2048
struct coursesData
{
  unsigned int id;
  char title[100];
  char url[100];
  double price;
  char level[20];
  double rating;
  char subject[50];
};

unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void newRecord(FILE *fp);
void deleteRecord(FILE *fp);
void filterLevel(FILE *fp);
void filterPrice(FILE *fp);
void searchCourses(FILE *fp);

int main()
{
  FILE *fp;
  if ((fp = fopen("udemy_courses.csv", "r")) == NULL)
  {
    puts("File could not be opened");
  }
  else
  {
    rewind(fp);
    unsigned int choice;
    while ((choice = enterChoice()) != 7)
    {
      switch (choice)
      {
      case 1:
        textFile(fp);
        break;
      case 2:
        newRecord(fp);
        break;
      case 3:
        deleteRecord(fp);
        break;
      case 4:
        filterLevel(fp);
        break;
      case 5:
        filterPrice(fp);
        break;
      case 6:
        searchCourses(fp);
      default:
        break;
      }
    }
  }
  return 0;
}

unsigned int enterChoice(void)
{
  printf("%s", "\nEnter your choice\n"
               "1 - Store a formatted text file of courses to load data from CSV file\n"
               "2 - Add a new course\n"
               "3 - Remove a course\n"
               "4 - Filter Levels\n"
               "5 - Filter Price\n"
               "6 - Search Courses\n"
               "7 - Exit Program\n"
               );
  unsigned int choice;
  scanf("%u", &choice);
  return choice;
}

void textFile(FILE *readPtr)
{
  FILE *writePtr;
  char fileName[50];
  printf("Please enter your file name to open\n");
  scanf("%49s", fileName);
  if ((writePtr = fopen(fileName, "w")) == NULL)
  {
    puts("File could not be opened");
  }
  else
  {
    fprintf(writePtr, "%s", "course_id,course_title,url,price,level,rating,subject\n");
    while (!feof(readPtr))
    {
      char buff[MAX_LINE]; // store first 1024 lines into buff
      int row_count = 0;
      int field_count = 0;
      struct coursesData courses[6000]; // array to structs to store values
      int i = 0;
      while (fgets(buff, 1024, readPtr))
      {
        field_count = 0;
        row_count++;
        if (row_count == 1)
          continue; // to skip header part
        char *field = strtok(buff, ",");
        // grab data
        while (field)
        {
          if (field_count == 0)
            courses[i].id = atoi(field); // string into int

          if (field_count == 1)
            strcpy(courses[i].title, field);

          if (field_count == 2)
            strcpy(courses[i].url, field);

          if (field_count == 3)
            courses[i].price = atof(field); // string into float

          if (field_count == 4)
            strcpy(courses[i].level, field);

          if (field_count == 5)
            courses[i].rating = atof(field); // string into float

          if (field_count == 6)
            strcpy(courses[i].subject, field);

          field = strtok(NULL, ",");
          field_count++;
        }
        i++;
      }

      // output values
      for (int i = 0; i < row_count - 1; i++)
      {
        fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                courses[i].rating, courses[i].subject);
      }
    }
  }
  rewind(readPtr);
  printf("\n File created successfully\n");
}

void newRecord(FILE *fp)
{
  // file open to append
  FILE *fappend = fopen("udemy_courses.csv", "a");
  if (fappend == NULL)
  {
    puts("File could not be opened");
  }
  else
  {
    rewind(fp);
    // get data from user
    struct coursesData course;
    printf("Enter course id :");
    scanf("%u", &course.id);
    printf("Enter title:");
    scanf(" %[^\n]s", course.title);
    printf("Enter url:");
    scanf(" %[^\n]s", course.url);
    printf("Enter price:");
    scanf("%lf", &course.price);
    printf("Enter level:");
    scanf(" %[^\n]s", course.level);
    printf("Enter rating:");
    scanf("%lf", &course.rating);
    printf("Enter subject:");
    scanf(" %[^\n]s", course.subject);
    bool index = true;
    while (!feof(fp))
    {
      char buff[1024]; // store first 1024 lines into buff
      int row_count = 0;
      int field_count = 0;
      struct coursesData courses[6000]; // array to structs to store values
      int i = 0;
      while (fgets(buff, 1024, fp))
      {
        field_count = 0;
        row_count++;
        if (row_count == 1)
          continue; // to skip header part
        char *field = strtok(buff, ",");
        // grab data
        while (field)
        {
          if (field_count == 0)
            courses[i].id = atoi(field); // string into int

          if (field_count == 1)
            strcpy(courses[i].title, field);

          if (field_count == 2)
            strcpy(courses[i].url, field);

          if (field_count == 3)
            courses[i].price = atof(field); // string into float

          if (field_count == 4)
            strcpy(courses[i].level, field);

          if (field_count == 5)
            courses[i].rating = atof(field); // string into float

          if (field_count == 6)
            strcpy(courses[i].subject, field);

          field = strtok(NULL, ",");
          field_count++;
        }
        i++;
      }

      // output values
      for (int i = 0; i < row_count - 1; i++)
      {
        if (courses[i].id == course.id)
        {
          index = false;
          break;
        }
      }
    }
    if (index == true)
    {
      fprintf(fappend, "%u,%s,%s,%.2lf,%s,%.2lf,%s\n", course.id,
              course.title, course.url, course.price, course.level,
              course.rating, course.subject);
      printf("\nYour course is successfully added!\n");
    }
    else if (index == false)
    {
      printf("Please try again! Choose different course id");
    }
    rewind(fp);
  }
  fclose(fappend);
}

void deleteRecord(FILE *fp)
{
  FILE *temp;
  char tmp_filename[FILENAME_SIZE];
  strcpy(tmp_filename, "temp____");
  strcat(tmp_filename, "udemy_courses.csv");
  temp = fopen(tmp_filename, "a");
  fprintf(temp, "%s", "course_id,course_title,url,price,level,rating,subject\n");

  int row_count = 0;
  int field_count = 0;
  struct coursesData courses[6000];
  unsigned int courseNum;
  printf("%s", "Enter your course number to delete record: ");
  scanf("%u", &courseNum);

  while (!feof(fp))
  {
    char buff[1024]; // store first 1024 lines into buff
    int row_count = 0;
    int field_count = 0;
    struct coursesData courses[6000]; // array to structs to store values
    int i = 0;
    while (fgets(buff, 1024, fp))
    {
      field_count = 0;
      row_count++;
      if (row_count == 1)
        continue; // to skip header part
      char *field = strtok(buff, ",");
      // grab data
      while (field)
      {
        if (field_count == 0)
          courses[i].id = atoi(field); // string into int

        if (field_count == 1)
          strcpy(courses[i].title, field);

        if (field_count == 2)
          strcpy(courses[i].url, field);

        if (field_count == 3)
          courses[i].price = atof(field); // string into float

        if (field_count == 4)
          strcpy(courses[i].level, field);

        if (field_count == 5)
          courses[i].rating = atof(field); // string into float

        if (field_count == 6)
          strcpy(courses[i].subject, field);

        field = strtok(NULL, ",");
        field_count++;
      }
      i++;
    }

    // output values
    for (int i = 0; i < row_count - 1; i++)
    {
      if (courses[i].id == courseNum)
      {
        continue;
      }
      fprintf(temp, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
              courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
              courses[i].rating, courses[i].subject);
    }
  }
  rewind(fp);
  remove("udemy_courses.csv");
  rename(tmp_filename, "udemy_courses.csv");
}

void filterLevel(FILE *fp)
{
  FILE *writePtr;
  char fileName[50];
  printf("Please enter your file name to save your filter courses\n");
  scanf("%49s", fileName);
  if ((writePtr = fopen(fileName, "w")) == NULL)
  {
    puts("File could not be opened");
  }
  else
  {
    fprintf(writePtr, "course_id,course_title,url,price,level,rating,subject\n");
    unsigned int num;
    printf("%s", "Choose 1 to display beginner level courses\n"
                 "- 2 to display intermediate level courses\n"
                 "- 3 to display all level courses\n");
    scanf("%u", &num);

    while (!feof(fp))
    {
      char buff[1024]; // store first 1024 lines into buff
      int row_count = 0;
      int field_count = 0;
      struct coursesData courses[6000]; // array to structs to store values
      int i = 0;
      while (fgets(buff, 1024, fp))
      {
        field_count = 0;
        row_count++;
        if (row_count == 1)
          continue; // to skip header part
        char *field = strtok(buff, ",");
        // grab data
        while (field)
        {
          if (field_count == 0)
            courses[i].id = atoi(field); // string into int

          if (field_count == 1)
            strcpy(courses[i].title, field);

          if (field_count == 2)
            strcpy(courses[i].url, field);

          if (field_count == 3)
            courses[i].price = atof(field); // string into float

          if (field_count == 4)
            strcpy(courses[i].level, field);

          if (field_count == 5)
            courses[i].rating = atof(field); // string into float

          if (field_count == 6)
            strcpy(courses[i].subject, field);

          field = strtok(NULL, ",");
          field_count++;
        }
        if (num == 1)
        {
          if (strstr((courses[i].level), "Beginner") != NULL)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
            i++;
          }
        }
        else if (num == 2)
        {
          if (strstr((courses[i].level), "Intermediate") != NULL)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
            i++;
          }
        }
        else if (num == 3)
        {
          if (strstr((courses[i].level), "All") != NULL)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
            i++;
          }
        }
      }
    }
    puts("\nFile created successfully\n");
    rewind(fp);
  }
}

void filterPrice(FILE *fp)
{
  FILE *writePtr;
  char fileName[50];
  printf("Please enter your file name to save your filter courses\n");
  scanf("%49s", fileName);
  if ((writePtr = fopen(fileName, "w")) == NULL)
  {
    puts("File could not be opened");
  }
  else
  {
    fprintf(writePtr, "course_id,course_title,url,price,level,rating,subject\n");
    double price;
    unsigned int input;
    printf("Choose 1 to get less than or equal to input price\n"
           "- 2 to get greater than or equal to input price\n");
    scanf("%u", &input);

    printf("Please enter highest price that you want to filter : ");
    scanf("%lf", &price);
    while (!feof(fp))
    {
      char buff[1024]; // store first 1024 lines into buff
      int row_count = 0;
      int field_count = 0;
      struct coursesData courses[6000]; // array to structs to store values
      int i = 0;
      while (fgets(buff, 1024, fp))
      {
        field_count = 0;
        row_count++;
        if (row_count == 1)
          continue; // to skip header part
        char *field = strtok(buff, ",");
        // grab data
        while (field)
        {
          if (field_count == 0)
            courses[i].id = atoi(field); // string into int

          if (field_count == 1)
            strcpy(courses[i].title, field);

          if (field_count == 2)
            strcpy(courses[i].url, field);

          if (field_count == 3)
            courses[i].price = atof(field); // string into float

          if (field_count == 4)
            strcpy(courses[i].level, field);

          if (field_count == 5)
            courses[i].rating = atof(field); // string into float

          if (field_count == 6)
            strcpy(courses[i].subject, field);

          field = strtok(NULL, ",");
          field_count++;
        }
        if (input == 1)
        {
          if (courses[i].price <= price)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
          }
          i++;
        }
        else if (input == 2)
        {
          if (courses[i].price >= price)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
          }
          i++;
        }
      }
    }
    puts("\nFile created successfully\n");
    rewind(fp);
  }
}

void searchCourses(FILE *fp)
{
  FILE *writePtr;
  char fileName[50];
  printf("Please enter your file name to save your search courses\n");
  scanf("%49s", fileName);
  if ((writePtr = fopen(fileName, "w")) == NULL)
  {
    puts("File could not be opened");
  }
  else
  {
    fprintf(writePtr, "course_id,course_title,url,price,level,rating,subject\n");
    unsigned int num;
    unsigned int course_id;
    char title[50];
    printf("%s", "Choose 1 to search by using course id\n"
                 "- 2 to search by using course title\n");
    scanf("%u", &num);
    switch (num)
    {
    case 1:
      printf("Enter your exact course id : ");
      scanf("%u", &course_id);
      break;
    case 2:
      printf("Enter your course title : ");
      scanf("%s", title);
      break;
    }
    while (!feof(fp))
    {
      char buff[1024]; // store first 1024 lines into buff
      int row_count = 0;
      int field_count = 0;
      struct coursesData courses[6000]; // array to structs to store values
      int i = 0;
      while (fgets(buff, 1024, fp))
      {
        field_count = 0;
        row_count++;
        if (row_count == 1)
          continue; // to skip header part
        char *field = strtok(buff, ",");
        // grab data
        while (field)
        {
          if (field_count == 0)
            courses[i].id = atoi(field); // string into int

          if (field_count == 1)
            strcpy(courses[i].title, field);

          if (field_count == 2)
            strcpy(courses[i].url, field);

          if (field_count == 3)
            courses[i].price = atof(field); // string into float

          if (field_count == 4)
            strcpy(courses[i].level, field);

          if (field_count == 5)
            courses[i].rating = atof(field); // string into float

          if (field_count == 6)
            strcpy(courses[i].subject, field);

          field = strtok(NULL, ",");
          field_count++;
        }
        if (num == 1)
        {
          if (courses[i].id == course_id)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
          }
          i++;
        }
        else if (num == 2)
        {
          if (strstr(courses[i].title, title) != NULL)
          {
            fprintf(writePtr, "%u,%s,%s,%.2lf,%s,%.2lf,%s",
                    courses[i].id, courses[i].title, courses[i].url, courses[i].price, courses[i].level,
                    courses[i].rating, courses[i].subject);
          }
          i++;
        }
      }
    }
    puts("\nFile created successfully\n");
    rewind(fp);
  }
}
