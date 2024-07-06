else if (strcmp(cmd_token, "fg") == 0)
            {
                if (noOfArgs < 2)
                {
                    error = 1;
                    printf("Error: Too few arguments\n");
                }
                else
                {
                    int jobNo = 0;
                    sscanf(cmd_token[1], "%d", &jobNo);

                    if (jobNo < 1 || jobNo > 99 || bgPid[jobNo] <= 1)
                    {
                        printf("Error: Invalid Job\n");
                    }
                    else
                    {
                        int pid = tichnas_fg(bgPid[jobNo]);

                        removeBg(pid > 0 ? pid : -pid);

                        if (pid <= 0)
                            error = 1;
                    }
                }
            }