#include <npth.h>

int
main (int argc, char *argv[])
{
  npth_mutex_t mutex;
  npth_init ();

  npth_mutex_init (&mutex, NULL);
  npth_mutex_lock (&mutex);
  npth_mutex_unlock (&mutex);
}

