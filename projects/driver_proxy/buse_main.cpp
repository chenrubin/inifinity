int buse_main(const char* dev_file, const struct buse_operations *aop, void *userdata)
{
  int sp[2];
  int nbd, sk, err, flags;

  err = socketpair(AF_UNIX, SOCK_STREAM, 0, sp);
  assert(!err);

  nbd = open(dev_file, O_RDWR);
  if (nbd == -1) {
    fprintf(stderr, 
        "Failed to open `%s': %s\n"
        "Is kernel module `nbd' loaded and you have permissions "
        "to access the device?\n", dev_file, strerror(errno));
    return 1;
  }

  if (aop->blksize) {
    err = ioctl(nbd, NBD_SET_BLKSIZE, aop->blksize);
    assert(err != -1);
  }
  if (aop->size) {
    err = ioctl(nbd, NBD_SET_SIZE, aop->size);
    assert(err != -1);
  }
  if (aop->size_blocks) {
    err = ioctl(nbd, NBD_SET_SIZE_BLOCKS, aop->size_blocks);
    assert(err != -1);
  }

  err = ioctl(nbd, NBD_CLEAR_SOCK);
  assert(err != -1);

  pid_t pid = fork();
  if (pid == 0) {
    /* Block all signals to not get interrupted in ioctl(NBD_DO_IT), as
     * it seems there is no good way to handle such interruption.*/
    sigset_t sigset;
    if (
      sigfillset(&sigset) != 0 ||
      sigprocmask(SIG_SETMASK, &sigset, NULL) != 0
    ) {
      warn("failed to block signals in child");
      exit(EXIT_FAILURE);
    }

    /* The child needs to continue setting things up. */
    close(sp[0]);
    sk = sp[1];

    if(ioctl(nbd, NBD_SET_SOCK, sk) == -1){
      fprintf(stderr, "ioctl(nbd, NBD_SET_SOCK, sk) failed.[%s]\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
    else{
#if defined NBD_SET_FLAGS
      flags = 0;
#if defined NBD_FLAG_SEND_TRIM
      flags |= NBD_FLAG_SEND_TRIM;
#endif
#if defined NBD_FLAG_SEND_FLUSH
      flags |= NBD_FLAG_SEND_FLUSH;
#endif
      if (flags != 0 && ioctl(nbd, NBD_SET_FLAGS, flags) == -1){
        fprintf(stderr, "ioctl(nbd, NBD_SET_FLAGS, %d) failed.[%s]\n", flags, strerror(errno));
        exit(EXIT_FAILURE);
      }
#endif
      err = ioctl(nbd, NBD_DO_IT);
      if (BUSE_DEBUG) fprintf(stderr, "nbd device terminated with code %d\n", err);
      if (err == -1) {
        warn("NBD_DO_IT terminated with error");
        exit(EXIT_FAILURE);
      }
    }

    if (
      ioctl(nbd, NBD_CLEAR_QUE) == -1 ||
      ioctl(nbd, NBD_CLEAR_SOCK) == -1
    ) {
      warn("failed to perform nbd cleanup actions");
      exit(EXIT_FAILURE);
    }

    exit(0);
  }

  /* Parent handles termination signals by terminating nbd device. */
  assert(nbd_dev_to_disconnect == -1);
  nbd_dev_to_disconnect = nbd;
  struct sigaction act;
  act.sa_handler = disconnect_nbd;
  act.sa_flags = SA_RESTART;
  if (
    sigemptyset(&act.sa_mask) != 0 ||
    sigaddset(&act.sa_mask, SIGINT) != 0 ||
    sigaddset(&act.sa_mask, SIGTERM) != 0
  ) {
    warn("failed to prepare signal mask in parent");
    return EXIT_FAILURE;
  }
  if (
    set_sigaction(SIGINT, &act) != 0 ||
    set_sigaction(SIGTERM, &act) != 0
  ) {
    warn("failed to register signal handlers in parent");
    return EXIT_FAILURE;
  }

  close(sp[1]);

  /* serve NBD socket */
  int status;
  status = serve_nbd(sp[0], aop, userdata);
  if (close(sp[0]) != 0) warn("problem closing server side nbd socket");
  if (status != 0) return status;

  /* wait for subprocess */
  if (waitpid(pid, &status, 0) == -1) {
    warn("waitpid failed");
    return EXIT_FAILURE;
  }
  if (WEXITSTATUS(status) != 0) {
    return WEXITSTATUS(status);
  }

  return EXIT_SUCCESS;
}