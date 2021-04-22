#include "pqueue.h"
#include <time.h>
#include <stdlib.h>

using namespace std;

int priorityFn1(const Patient &patient);
int priorityFn2(const Patient &patient);

const string RED_TEXT = "\033[1;31m";
const string GREEN_TEXT = "\033[0;32m";
const string RESET_TEXT = "\033[0m";
const int MAX_TEST = 4000;

int main() {
    srand (time(NULL));

    PQueue queue1(priorityFn1);
    PQueue queue2(priorityFn1);
    PQueue queue3(priorityFn2);
    PQueue emptyQueue(priorityFn1);

    Patient patient1("Sam", 1, 37, 79,  36, 133, 75);
    Patient patient2("Liz", 5, 37, 98, 26,  70, 100);
    Patient patient3("Nick", 2, 37, 85, 34, 125, 85);
    Patient patient4("Eva", 4, 37, 95,  22, 115, 95);
    Patient patient5("Bart", 3, 37, 95,  22, 115, 95);
    Patient patient6("James", 2, 37, 95,  22, 115, 95);
    Patient patient7("Ben", 5, 37, 95,  22, 115, 95);
    Patient patient8("Natolya", 2, 37, 95,  22, 115, 95);
    Patient patient9("Chris", 1, 37, 95,  22, 115, 95);
    Patient patient10("Sean", 5, 37, 95,  22, 115, 95);
    Patient patient11("Jimmy", 3, 37, 95,  22, 115, 95);
    
    // Make sure constructor assigns size to 0
    cout << "\n1. Testing constructor for size parameter" << endl;
    if(queue1.numPatients() == 0){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    queue1.insertPatient(patient5);
    queue1.insertPatient(patient3);
    // Test size changing properly on insertion
    cout << "\n2. Changing size when inserting" << endl;
    if(queue1.numPatients() == 2){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    queue2.insertPatient(patient8);
    queue2.insertPatient(patient2);
    queue2.insertPatient(patient1);
    // Merging with another queue w/ elements and checking size
    queue1.mergeWithQueue(queue2);
    cout << "\n3. Changing size when merging with another full queue" << endl;
    if(queue1.numPatients() == 5){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Testing merging with an empty queue
    queue1.mergeWithQueue(emptyQueue);
    cout << "\n4. Testing size after merging w/ empty queue" << endl;
    if(queue1.numPatients() == 5){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    queue3.insertPatient(patient1);
    queue3.insertPatient(patient2);
    // Testing merging with a queue w/ different priority function
    cout << "\n5. Testing size after merging w/ different priority (will throw error)" << endl;
    try{
        queue1.mergeWithQueue(queue3);
    }
    catch(const domain_error& e){
        cout << "domain_error caught with message: " << e.what() << endl;
    }

    cout << "\n6. Checking structure of queue1 after using getNextPatient()" << endl;
    cout << "Before:" << endl;
    queue1.dump();
    cout << endl;
    // Test getNextPatient changes heap and keeps structure
    queue1.getNextPatient();
    cout << "After:" << endl; 
    queue1.dump();
    cout << endl;

    // Check that size decremented after using getNextPatient()
    cout << "\n7. Checking size of queue1 decrements after using getNextPatient()" << endl;
    if(queue1.numPatients() == 4){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Test rebuild when priority fctn changed
    cout << "\n8. Updating priorityFn and checking output" << endl;
    cout << "Before:" << endl;
    queue1.printPatientQueue();
    cout << "After:" << endl;
    queue1.setPriorityFn(priorityFn2);
    queue1.printPatientQueue();

    cout << "\n9. Making sure size is maintained after rebuilding with setPriorityFn()" << endl;
    if(queue1.numPatients() == 4){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Try to getNextPatient() on an empty queue
    cout << "\n10. Test getNextPatient() on empty queue (will throw exception)" << endl;
    try{
        emptyQueue.getNextPatient();
    }
    catch(const domain_error& e){
        cout << "domain_error caught with message: " << e.what() << endl;
    }

    // Testing setPriorityFn() on an empty queue
    cout << "\n11. Test setPriorityFn() on empty queue (should not crash)" << endl;
    emptyQueue.setPriorityFn(priorityFn2);
    emptyQueue.dump();
    cout << "Did not crash." << endl;

    // Test copy constructor on full queue
    cout << "\n12. Making new queue from copy constructor (with full queue)" << endl;
    PQueue newQueue(queue1);
    cout << "newQueue contents:" << endl;
    newQueue.dump();
    cout << endl << "queue1 contents:" << endl;
    queue1.dump();
    cout << endl;

    cout << "\n13. Checking size transferred to newQueue" << endl;
    if(newQueue.numPatients() == 4 && queue1.numPatients() == 4){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Testing deep copy - queue1 should change but newQueue should not
    cout << "\n14. Removing top priority from queue1 but not newQueue (results should be different)" << endl;
    queue1.getNextPatient();
    cout << "newQueue contents:" << endl;
    newQueue.dump();
    cout << endl << "queue1 contents:" << endl;
    queue1.dump();
    cout << endl;
    
    cout << "\n15. Checking size changed from queue1 but not newQueue" << endl;
        if(newQueue.numPatients() == 4 && queue1.numPatients() == 3){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Test copy constructor on empty queue
    cout << "\n16. Creating new PQueue from empty queue" << endl;
    PQueue newEmptyQueue(emptyQueue);
    cout << "newEmptyQueue contents:" << endl;
    newEmptyQueue.dump();
    cout << endl << "emptyQueue (original) contents:" << endl;
    emptyQueue.dump();
    cout << endl;

    cout << "\n17. Checking sizes of both are still 0" << endl;
    if(newEmptyQueue.numPatients() == 0 && emptyQueue.numPatients() == 0){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Creating new queue for assignment op
    PQueue queue4(priorityFn1);
    queue4.insertPatient(patient11);
    queue4.insertPatient(patient1);
    queue4.insertPatient(patient3);
    queue4.insertPatient(patient5);
    queue4.insertPatient(patient10);
    queue4.insertPatient(patient7);
    queue4.insertPatient(patient8);
    queue4.insertPatient(patient9);

    // Test assignment op on full queue
    cout << "\n17. Creating new queue from queue1 using assignment op." << endl;

    PQueue freshQueue(priorityFn1);
    freshQueue = queue4;
    cout << endl << "freshQueue contents:" << endl;
    freshQueue.dump();
    cout << endl << "queue1 (original) contents:" << endl;
    queue4.dump();
    cout << endl << endl;

    // Check size
    cout << "18. Checking size for previous operation" << endl;
    if(queue4.numPatients() == 8 && freshQueue.numPatients() == 8){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Test for deep copy
    cout << "\n19. Removing top priority from queue4 but not freshQueue (results should be different)" << endl;
    queue4.getNextPatient();
    cout << "freshQueue contents:" << endl;
    freshQueue.dump();
    cout << endl << "queue4 contents:" << endl;
    queue4.dump();
    cout << endl;
    
    cout << "\n20. Checking size changed from queue4 but not newQueue" << endl;
        if(freshQueue.numPatients() == 8 && queue4.numPatients() == 7){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Test assigning to a queue that already has items in it
    cout << "\n21. Assigning to a queue with nodes already (freshQueue should now be = to queue1)" << endl;
    freshQueue = queue1;
    cout << "freshQueue contents:" << endl;
    freshQueue.dump();
    cout << endl << "queue1 contents:" << endl;
    queue1.dump();
    cout << endl;

    // Check size
    cout << "\n22. Checking sizes from previous operation" << endl;
    if(freshQueue.numPatients() == 3 && queue1.numPatients() == 3){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Deep copy again
    cout << "\n23. Removing top priority from queue1 but not freshQueue (results should be different)" << endl;
    queue1.getNextPatient();
    cout << "freshQueue contents:" << endl;
    freshQueue.dump();
    cout << endl << "queue4 contents:" << endl;
    queue1.dump();
    cout << endl;
    
    cout << "\n24. Checking size changed from queue1 but not newQueue" << endl;
    if(freshQueue.numPatients() == 3 && queue1.numPatients() == 2){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Test assignment op on empty queue
    cout << "\n25. Assigning full queue to an empty queue (should print no contents)" << endl;
    queue1 = emptyQueue;
    cout << "emptyQueue contents:" << endl;
    emptyQueue.dump();
    cout << endl << "queue1 contents:" << endl;
    queue1.dump();
    cout << endl;

    // Check size
    cout << "\n25. Checking size changed from queue1 but not newQueue" << endl;
    if(emptyQueue.numPatients() == 0 && queue1.numPatients() == 0){
        cout << GREEN_TEXT << "Test passed" << RESET_TEXT << endl;
    }
    else{
        cout << RED_TEXT << "Test failed" << RESET_TEXT << endl;
    }

    // Test for self assignment (shouldn't do anything)
    cout << "\n26. Self assignment check" << endl;
    cout << "Contents before: " << endl;
    queue4.dump();
    queue4 = queue4;
    cout << endl << "Contents after: " << endl;
    queue4.dump();
    cout << endl;

    cout << "\n27. Self merge check" << endl;
    cout << "Contents before: " << endl;
    queue4.dump();
    queue4.mergeWithQueue(queue4);
    cout << endl << "Contents after: " << endl;
    queue4.dump();
    cout << endl;
    
    // UNCOMMENT TO TEST WITH BIGGER NUMBERS
    // PQueue stressQueue(priorityFn2);

    // for (int i = 0; i < MAX_TEST; i++){
    //     int randNum = (rand() % 5) + 1;
    //     Patient patient20(to_string(i), randNum, 37, 79,  36, 133, 75);
    //     stressQueue.insertPatient(patient20);
    // }
    // stressQueue.dump();

    // cout << endl<< endl << endl << "Removing root until size = 0" << endl;
    // int counter = 1;
    // while (stressQueue.numPatients() > 0) {
    //     if(counter == 4000){
    //         cout << counter << " " << endl;
    //     }
    //     stressQueue.getNextPatient();
    //     counter++;
    // }
}

int priorityFn1(const Patient&patient) {
  int pri = 5;
  if ((patient.getOxygen()<80 && (patient.getRR() > 35 || patient.getRR() < 8)) ||
      (patient.getHR() > 130 && patient.getBP() < 80))
      pri = 1;
  else if (((patient.getOxygen() >= 80 && patient.getOxygen() <= 89) && 
      (patient.getRR() >= 31 && patient.getRR() <= 35)) ||
      (patient.getTemperature() >= 40 || patient.getTemperature() <= 32))
      pri = 2;
  else if ((patient.getOxygen() >= 90 && patient.getOxygen() <= 94) && 
      (patient.getRR() >= 26 && patient.getRR() <= 30))
      pri = 3; 
  else if ((patient.getOxygen() >= 95 && patient.getOxygen() <= 97) && 
      (patient.getRR() >= 21 && patient.getRR() <= 25))
      pri = 4;
  else
      pri = 5;
  return pri;
}

int priorityFn2(const Patient&patient) {
  //priority only based on triage
  //traige is set by a health care professional
  return patient.getTriage();
}