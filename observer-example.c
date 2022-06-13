/* This is a code snippet that I wrote to implement observer pattern in C
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct _observer {
    /**
     * Method for performing garbage collection
     */
    void (*destroy)(struct _observer *);
    /**
     * A reference to the concrete OBSERVER
     */
    void* impl;

} Observer;

typedef struct _specificObserver
{
   Observer observer;
   
   void (*specificMethod)(Observer*, int, char*);
} SpecificObserver;

#define NOTIFY_OBSERVERS(observable, type, method, ...)     \
{int i = 0;                                         \
type* observer_type;                                 \
for (; i < MAX_OBSERVERS; i++) {                    \
    if (observable->observers[i] != NULL) {               \
        observer_type = (type*) observable->observers[i];    \
        observer_type->method(observable->observers[i], ##__VA_ARGS__); \
    } \
}}\

#define MAX_OBSERVERS 5

typedef struct _observable
{
    /**
     * Referente to the concrete Observable object
     */
    void * impl;
    /**
     * A list of observes have ben are registered for this subject
     * object.
     */
    Observer * observers[MAX_OBSERVERS];
    /**
     * The method that allows observers to register for this
     * subject object.
     */
    int (*registerObserver)(struct _observable*, Observer*);
    /**
     * Lets observes unregister from this subject
     */
    int (*unregisterObserver)(struct _observable *, Observer*);
    /**
     * When fired, triggers notify method of all of the registered
     * observers.
     */
    void (*notifyObservers)(struct _observable*, ...);
} Observable;


static int _registerObserver(Observable* this, Observer* observer)
{
    int i = 0;

    for (; i < MAX_OBSERVERS; i++) {
        if (this->observers[i] == NULL) {
            this->observers[i] = observer;

            return EXIT_SUCCESS;
        }
    }

    printf("[INF] [SUBJECT] we have rush the max number of observers\n");
    return EXIT_FAILURE;
}

static int _unregisterObserver(Observable *this, Observer* observer)
{
    int i = 0;

    for (; i < MAX_OBSERVERS; i++) {
        void* pObserver = this->observers[i];

        if (observer == pObserver) {
            pObserver = NULL;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

Observable* newObservable(void* impl)
{
    Observable* this = (Observable *) malloc(sizeof(Observable));

    this->impl = impl;
    this->registerObserver = _registerObserver;
    this->unregisterObserver = _unregisterObserver;

    return this;
}

SpecificObserver* newSpecificObserver(void* impl, void (*method)(Observer*, int, char*))
{
    SpecificObserver* this = (SpecificObserver *) malloc(sizeof(SpecificObserver));

    this->observer.impl = impl;
    this->specificMethod = method;

    return this;
}

void notificacion(Observer* observer, int param, char* text)
{
    printf("\nNotification received %d %s", param, text);
}

int main()
{
    SpecificObserver* observer1;
    Observable* observable1;

    observer1 = newSpecificObserver(NULL, notificacion);
    observable1 = newObservable(NULL);
  
    _registerObserver(observable1, (Observer*)observer1);

    NOTIFY_OBSERVERS(observable1, SpecificObserver, specificMethod, 1, "hola caracola");

    printf("\nsquare %d", 2);

    return 0;

}
