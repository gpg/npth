/* npth.c - a lightweight implementation of pth over pthread.
   Copyright (C) 2011 g10 Code GmbH

   This file is part of NPTH.

   NPTH is free software; you can redistribute it and/or modify it
   under the terms of either

   - the GNU Lesser General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at
   your option) any later version.

   or

   - the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at
   your option) any later version.

   or both in parallel, as here.

   NPTH is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copies of the GNU General Public License
   and the GNU Lesser General Public License along with this program;
   if not, see <http://www.gnu.org/licenses/>.  */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "npth.h"


#include <stdio.h>
#define DEBUG_CALLS 1
#define _npth_debug(x, ...) printf(__VA_ARGS__)

#ifndef TEST
#undef  DEBUG_CALLS
#define DEBUG_CALLS 0
#undef _npth_debug
#define _npth_debug(x, ...)
#endif

/* The global lock that excludes all threads but one.  This is a
   semaphore, because these can be safely used in a library even if
   the application or other libraries call fork(), including from a
   signal handler.  sem_post is async-signal-safe.  (The reason a
   semaphore is safe and a mutex is not safe is that a mutex has an
   owner, while a semaphore does not.)  */
static sem_t sceptre;

static pthread_t main_thread;


static void
enter_npth (const char *function)
{
  int res;

  if (DEBUG_CALLS)
    _npth_debug (DEBUG_CALLS, "enter_npth (%s)\n",
		 function ? function : "unknown");
  res = sem_post (&sceptre);
  assert (res == 0);
}


static void
leave_npth (const char *function)
{
  int res;

  do {
    res = sem_wait (&sceptre);
  } while (res < 0 && errno == EINTR);

  assert (!res);

  if (DEBUG_CALLS)
    _npth_debug (DEBUG_CALLS, "leave_npth (%s)\n",
		 function ? function : "");
}

#define ENTER() enter_npth(__FUNCTION__)
#define LEAVE() leave_npth(__FUNCTION__)

int
npth_init (void)
{
  int res;

  main_thread = pthread_self();

  /* The semaphore is not shared and binary.  */
  sem_init(&sceptre, 0, 1);
  if (res < 0)

  LEAVE();
  return 0;
}


struct startup_s
{
  void *(*start_routine) (void *);
  void *arg;
};


static void *
thread_start (void *startup_arg)
{
  struct startup_s *startup = startup_arg;
  void *(*start_routine) (void *);
  void *arg;
  void *result;

  start_routine = startup->start_routine;
  arg = startup->arg;
  free (startup);

  LEAVE();
  result = (*start_routine) (arg);
  /* Note: instead of returning here, we might end up in
     npth_exit() instead.  */
  ENTER();

  return result;
}


int
npth_create (npth_t *thread, const npth_attr_t *attr,
	     void *(*start_routine) (void *), void *arg)
{
  int err;
  struct startup_s *startup;

  startup = malloc (sizeof (*startup));
  if (!startup)
    return errno;

  startup->start_routine = start_routine;
  startup->arg = arg;
  err = pthread_create (thread, attr, thread_start, startup);
  if (err)
    {
      free (startup);
      return err;
    }

  /* Memory is released in thread_start.  */
  return 0;
}


int
npth_join (npth_t thread, void **retval)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_tryjoin_np (thread, retval);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_join (thread, retval);
  LEAVE();
  return err;
}


void
npth_exit (void *retval)
{
  ENTER();
  pthread_exit (retval);
  /* Never reached.  But just in case pthread_exit does return... */
  LEAVE();
}


int
npth_mutex_lock (npth_mutex_t *mutex)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_mutex_trylock (mutex);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_mutex_lock (mutex);
  LEAVE();
  return err;
}


int
npth_mutex_timedlock (npth_mutex_t *mutex, const struct timespec *abstime)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_mutex_trylock (mutex);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_mutex_timedlock (mutex, abstime);
  LEAVE();
  return err;
}


int
npth_rwlock_rdlock (npth_rwlock_t *rwlock)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_rwlock_tryrdlock (rwlock);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_rwlock_rdlock (rwlock);
  LEAVE();
  return err;
}


int
npth_rwlock_timedrdlock (npth_rwlock_t *rwlock, const struct timespec *abstime)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_rwlock_tryrdlock (rwlock);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_rwlock_timedrdlock (rwlock, abstime);
  LEAVE();
  return err;
}


int
npth_rwlock_wrlock (npth_rwlock_t *rwlock)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_rwlock_trywrlock (rwlock);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_rwlock_wrlock (rwlock);
  LEAVE();
  return err;
}


int
npth_rwlock_timedwrlock (npth_rwlock_t *rwlock, const struct timespec *abstime)
{
  int err;

  /* No need to allow competing threads to enter when we can get the
     lock immediately.  */
  err = pthread_rwlock_trywrlock (rwlock);
  if (err != EBUSY)
    return err;

  ENTER();
  err = pthread_rwlock_timedwrlock (rwlock, abstime);
  LEAVE();
  return err;
}


int
npth_cond_wait (npth_cond_t *cond, npth_mutex_t *mutex)
{
  int err;

  ENTER();
  err = pthread_cond_wait (cond, mutex);
  LEAVE();
  return err;
}


int
npth_cond_timedwait (npth_cond_t *cond, npth_mutex_t *mutex,
		     const struct timespec *abstime)
{
  int err;

  ENTER();
  err = pthread_cond_timedwait (cond, mutex, abstime);
  LEAVE();
  return err;
}


/* Standard POSIX Replacement API */

int
npth_usleep(unsigned int usec)
{
  int res;

  ENTER();
  res = usleep(usec);
  LEAVE();
  return res;
}


unsigned int
npth_sleep(unsigned int sec)
{
  unsigned res;

  ENTER();
  res = sleep(sec);
  LEAVE();
  return res;
}


int
npth_system(const char *cmd)
{
  int res;

  ENTER();
  res = system(cmd);
  LEAVE();
  return res;
}


pid_t
npth_waitpid(pid_t pid, int *status, int options)
{
  pid_t res;

  ENTER();
  res = waitpid(pid,status, options);
  LEAVE();
  return res;
}


int
npth_connect(int s, const struct sockaddr *addr, socklen_t addrlen)
{
  int res;

  ENTER();
  res = connect(s, addr, addrlen);
  LEAVE();
  return res;
}


int
npth_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
  int res;

  ENTER();
  res = accept(s, addr, addrlen);
  LEAVE();
  return res;
}


int
npth_select(int nfd, fd_set *rfds, fd_set *wfds, fd_set *efds,
	    struct timeval *timeout)
{
  int res;

  ENTER();
  res = select(nfd, rfds, wfds, efds, timeout);
  LEAVE();
  return res;
}


int
npth_pselect(int nfd, fd_set *rfds, fd_set *wfds, fd_set *efds,
	     const struct timespec *timeout, const sigset_t *sigmask)
{
  int res;

  ENTER();
  res = pselect(nfd, rfds, wfds, efds, timeout, sigmask);
  LEAVE();
  return res;
}


ssize_t
npth_read(int fd, void *buf, size_t nbytes)
{
  ssize_t res;

  ENTER();
  res = read(fd, buf, nbytes);
  LEAVE();
  return res;
}


ssize_t
npth_write(int fd, const void *buf, size_t nbytes)
{
  ssize_t res;

  ENTER();
  res = write(fd, buf, nbytes);
  LEAVE();
  return res;
}


int
npth_recvmsg (int fd, struct msghdr *msg, int flags)
{
  int res;

  ENTER();
  res = recvmsg (fd, msg, flags);
  LEAVE();
  return res;
}


int
npth_sendmsg (int fd, const struct msghdr *msg, int flags)
{
  int res;

  ENTER();
  res = sendmsg (fd, msg, flags);
  LEAVE();
  return res;
}


void
npth_unprotect (void)
{
  ENTER();
}


void
npth_protect (void)
{
  LEAVE();
}
