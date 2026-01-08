// Platform compatibility shims for Windows
#ifdef _WIN32
#include <direct.h>
#define popen _popen
#define pclose _pclose
#define chdir _chdir
#define mkdir _mkdir
#endif

/*
 This file contains all the callBack functions and functions that it calls to do its work.
 This file contains all the ways a user can interact (Mouse and Terminal) with 
 a running simulation.
 
 The functions in this file are listed below and in this order.
 
 void reshape(GLFWwindow* window, int width, int height);
 void mouseFunctionsOff();
 void mouseAblateMode();
 void mouseEctopicBeatMode();
 void mouseAdjustMusclesAreaMode();
 void mouseAdjustMusclesLineMode();
 void mouseIdentifyNodeMode();
 bool setMouseMuscleAttributes();
 void setEctopicBeat(int nodeId);
 void clearStdin();
 string getTimeStamp();
 void movieOn();
 void movieOff();
 void screenShot();
 void saveSettings();
 void saveState();
 void loadState();
 void findNodes();
 void KeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
 void keyHeld(GLFWwindow* window);
 void mousePassiveMotionCallback(GLFWwindow* window, double x, double y);
 void myMouse(GLFWwindow* window, int button, int state, double x, double y);
 void scrollWheel(GLFWwindow*, double, double);

/*
 OpenGL callback when the window is reshaped.
*/
void reshape(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // Set the viewport size to match the window size

	//calculate the image aspect ratio
	float aspect = (float)width / (float)height;

	//set the projection matrix -- this is basically the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//now we need to maintain the same aspect ratio for both orthogonal and frustum view
	if(Simulation.ViewFlag == 0) // Orthogonal view
	{
		glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0); // Orthogonal projection
	}
	else // Frustum view
	{
		glFrustum(-aspect, aspect, -1.0, 1.0, 1.0, 100.0); // Frustum projection
	}

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity(); //don't need this because it resets the camera every time we reshape
}

/*
 Turns off all the user interactions.
*/
void mouseFunctionsOff()
{
	//Simulation.isPaused = true;
	Simulation.isInAblateMode = false;
	Simulation.isInEctopicBeatMode = false;
	Simulation.isInEctopicEventMode = false;
	Simulation.isInAdjustMuscleAreaMode = false;
	Simulation.isInAdjustMuscleLineMode = false;
	Simulation.isInFindNodeMode = false;
	Simulation.isInMouseFunctionMode = false;
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Set cursor to default arrow.
	drawPicture();
}

/*
 Puts the user in ablate mode.
*/
void mouseAblateMode()
{
	mouseFunctionsOff();
	Simulation.isPaused = true;
	Simulation.isInAblateMode = true;
	Simulation.isInMouseFunctionMode = true;
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Set cursor to hidden.
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//orthogonalView();
	drawPicture();
}

/*
 Puts the user in ectopic beat mode.
*/
void mouseEctopicBeatMode()
{
	mouseFunctionsOff();
	Simulation.isPaused = true;
	Simulation.isInEctopicBeatMode = true;
	Simulation.isInMouseFunctionMode = true;
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Set cursor to hidden.
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//orthogonalView();
	drawPicture();
}

/*
 Puts the user in ectopic event mode.
*/
void mouseEctopicEventMode()
{
	mouseFunctionsOff();
	Simulation.isPaused = true;
	Simulation.isInEctopicEventMode = true;
	Simulation.isInMouseFunctionMode = true;
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Set cursor to hidden.
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//orthogonalView();
	drawPicture();
}

/*
 Puts the user in area muscle adjustment mode.
*/
void mouseAdjustMusclesAreaMode()
{
	mouseFunctionsOff();
	Simulation.isPaused = true;
	Simulation.isInAdjustMuscleAreaMode = true;
	Simulation.isInMouseFunctionMode = true;
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Set cursor to hidden.
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//orthogonalView();
	drawPicture();
	
	//bool returnFlag = setMouseMuscleAttributes();
}

/*
 Puts the user in line muscle adjustment mode.
*/
void mouseAdjustMusclesLineMode()
{
	mouseFunctionsOff();
	Simulation.isPaused = true;
	Simulation.isInAdjustMuscleLineMode = true;
	Simulation.isInMouseFunctionMode = true;
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Set cursor to hidden.
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//orthogonalView();
	drawPicture();
	
	//bool returnFlag = setMouseMuscleAttributes();
	
}

/*
 Puts the user in identify node mode.
*/
void mouseIdentifyNodeMode()
{
	mouseFunctionsOff();
	Simulation.isPaused = true;
	Simulation.isInFindNodeMode = true;
	Simulation.isInMouseFunctionMode = true;
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Set cursor to hidden.
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//orthogonalView();
	drawPicture();
}

/*
	Calls the functions that get user inputs for modifying the refractory periods 
	and conduction velocities of the selected muscles
*/
// bool setMouseMuscleAttributes()
// {
// 	// These functions now just set default values
// 	//RefractoryPeriodAdjustmentMultiplier = 1.0;
// 	//MuscleConductionVelocityAdjustmentMultiplier = 1.0;
// 	return(true);
// }

/*
 This function sets up a node (nodeId) to be an ectopic beat node.
*/
void setEctopicBeat(int nodeId)
{
	Node[nodeId].isBeatNode = true;
	
	if(!Node[nodeId].isAblated)
	{
		Node[nodeId].isDrawNode = true;
		Node[nodeId].color.x = 1.0;
		Node[nodeId].color.y = 1.0;
		Node[nodeId].color.z = 0.0;
	}
	drawPicture();
	
	// Set default values - these used to come from user input functions
	Node[nodeId].beatPeriod = BeatPeriod; // Default to same as main beat
	Node[nodeId].beatTimer = 0; // Default to start immediately
	
	
	// We only let you set 1 ectopic beat at a time.
	Simulation.isInEctopicBeatMode = false;
}

/*
 This function is used to clear the print buffer.
*/
void clearStdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* discard characters */
    }
}


/*
 This function returns a timestamp in M-D-Y-H.M.S format.
 This is use so each file that is created has a unique name. 
 Note: You cannot create more than one file in a second or you will over write the previous file.
*/
string getTimeStamp()
{
	// Want to get a time stamp string representing current date/time, so we have a
	// unique name for each video/screenshot taken.
	time_t t = time(0); 
	struct tm * now = localtime( & t );
	int month = now->tm_mon + 1, day = now->tm_mday, year = now->tm_year, 
				curTimeHour = now->tm_hour, curTimeMin = now->tm_min, curTimeSec = now->tm_sec;

	stringstream smonth, sday, syear, stimeHour, stimeMin, stimeSec;

	smonth << month;
	sday << day;
	syear << (year + 1900); // The computer starts counting from the year 1900, so 1900 is year 0. So we fix that.
	stimeHour << curTimeHour;
	stimeMin << curTimeMin;
	stimeSec << curTimeSec;
	string timeStamp;

	if (curTimeMin <= 9)
	{
		timeStamp = smonth.str() + "-" + sday.str() + "-" + syear.str() + '_' + stimeHour.str() + ".0" + stimeMin.str() + 
					"." + stimeSec.str();
	}
	else
	{		
		timeStamp = smonth.str() + "-" + sday.str() + '-' + syear.str() + "_" + stimeHour.str() + "." + stimeMin.str() +
					"." + stimeSec.str();
	}

	return timeStamp;
}

/*
 This function turns the movie capture on.
*/
void movieOn()
{
	string ts = getTimeStamp();
	ts.append(".mp4");

	// Setting up the movie buffer.
	/*const char* cmd = "ffmpeg -loglevel quiet -r 60 -f rawvideo -pix_fmt rgba -s 1000x1000 -i - "
		      "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";*/

	char baseCommand[512]; // Command to run ffmpeg with the correct parameters for capturing a movie
	//use sprintf to create the command string for ffmpeg, used XWindowSize and YWindowSize to set the size of the image

	//Low Quality, Fast Speed, Small Size
	// sprintf(baseCommand, "ffmpeg -loglevel quiet -r 60 -f rawvideo -pix_fmt rgba -s %dx%d -i - "
	// 	"-c:v libx264 -threads 0 -preset fast -y -pix_fmt yuv420p -crf 0 -vf vflip \"%s\"", XWindowSize, YWindowSize, ts.c_str());

	//Medium Quality, Medium Speed, Medium Size
	// sprintf(baseCommand, "ffmpeg -loglevel quiet -r 60 -f rawvideo -pix_fmt rgba -s %dx%d -i - "
	// 			"-c:v libx264 -threads 0 -preset medium -y -pix_fmt yuv420p -crf 0 -vf vflip \"%s\"", XWindowSize, YWindowSize, ts.c_str());

	//Max Quality, Low Speed, Large Size (change crf to 0 range[0,51] for lossless compression, but I wanted to keep the file size down)
	sprintf(baseCommand, "ffmpeg -loglevel quiet -r 60 -f rawvideo -pix_fmt rgba -s %dx%d -i - "
		"-c:v libx264 -threads 0 -preset veryslow -y -crf 16 -tune film -vf vflip \"%s\"", XWindowSize, YWindowSize, ts.c_str());

	//use the command string to create the output file name
	MovieFile = popen(baseCommand, "w");

	//Buffer = new int[XWindowSize*YWindowSize];
	Buffer = (unsigned char*)malloc(4* XWindowSize* YWindowSize);

	Simulation.isRecording = true;
}

/*
 This function turns the movie capture off.
*/
void movieOff()
{
	if(Simulation.isRecording) 
	{
		pclose(MovieFile);
	}
	free(Buffer);
	Simulation.isRecording = false;
}

/*
 This function takes a screen shot of the simulation.
*/
void screenShot()
{	
	bool savedPauseState;
	FILE* ScreenShotFile;
	unsigned char* buffer; //unsigned char because we are using RGBA data, which is 4 bytes per pixel, 1 char = 1 byte

    char cmd[512]; // Command to run ffmpeg with the correct parameters for capturing a screenshot

	//commands for ffmpeg, used XWindowSize and YWindowSize to set the size of the image
    sprintf(cmd, "ffmpeg -loglevel quiet -framerate 60 -f rawvideo -pix_fmt rgba -s %dx%d -i - "
                "-c:v libx264rgb -threads 0 -preset fast -y -crf 0 -vf vflip output1.mp4", 
                XWindowSize, YWindowSize);

	//SC 25 submission
	//sprintf(cmd, "ffmpeg -loglevel quiet -framerate 60 -f rawvideo -pix_fmt rgba -s 3840x2160 -i - -c:v libx264rgb -threads 0 -preset fast -y -crf 0 -vf vflip output1.mp4");

	//const char* cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1000x1000 -i - "
	//              "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output1.mp4";
	
	//open the pipe to ffmpeg and allocate the buffer for the screenshot with the size of 4*XWin* YWin to hold the RGBA data
	ScreenShotFile = popen(cmd, "w"); // Open pipe for screenshot command
	buffer = (unsigned char*)malloc(4 * XWindowSize*YWindowSize*sizeof(int));
	
	if(!Simulation.isPaused) //if the simulation is running
	{
		Simulation.isPaused = true; //pause the simulation
		savedPauseState = false; //save the pause state
	}
	else //if the simulation is already paused
	{
		savedPauseState = true; //save the pause state
	}
	
	for(int i =0; i < 1; i++)
	{
		drawPicture();
		glReadPixels(0, 0, XWindowSize, YWindowSize, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		fwrite(buffer, 4*XWindowSize*YWindowSize, 1, ScreenShotFile);
	}
	
	pclose(ScreenShotFile);
	free(buffer);

	string ts = getTimeStamp(); // Only storing in a separate variable for debugging purposes.
	string s = "ffmpeg -loglevel quiet -i output1.mp4 -qscale:v 1 -qmin 1 -qmax 1 " + ts + ".jpeg";
	//Below is for SC25 submission
	//"ffmpeg -loglevel quiet -i output1.mp4 -qscale:v 1 -qmin 1 -qmax 1 " + ts + ".png && convert " + ts + ".png -density 72 " + ts + ".png";
	// Convert back to a C-style string.
	const char *ccx = s.c_str();
	system(ccx);
	// Portable delete of temporary file
	remove("output1.mp4"); // Use remove instead of system call for portability
	printf("\nScreenshot Captured: \n");
	cout << "Saved as " << ts << ".jpeg" << endl;

	
	//system("ffmpeg -i output1.mp4 screenShot.jpeg");
	//system("rm output1.mp4");

	Simulation.isPaused = savedPauseState; //restore the pause state before we took the screenshot
	//ffmpeg -i output1.mp4 output_%03d.jpeg
}


/*
 This function saves all the node and muscle values set in the run to a file. This file can then be used at a
 later date to start a run with the exact settings used at the time of capture.
 So if the user has spent a great deal of time setting up a scenario, they can save the scenario and use it again later.
 We use it to create scenarios that have arrhythmias preprogrammed into them and have members from a class we are
 presenting to come up and see if they can use the ablation tool to eliminate the arythmia.
*/
void saveSettings()
{
	// Copying the latest node and muscle information down from the GPU.
	cudaMemcpy( Node, NodeGPU, NumberOfNodes*sizeof(nodeAttributesStructure), cudaMemcpyDeviceToHost);
	cudaErrorCheck(__FILE__, __LINE__);
	cudaMemcpy( Muscle, MuscleGPU, NumberOfMuscles*sizeof(muscleAttributesStructure), cudaMemcpyDeviceToHost);
	cudaErrorCheck(__FILE__, __LINE__);
	
	// Moving into the file that contains previous run files.
	chdir("./PreviousRunsFile"); // Change directory to previous runs
	   	
	// Creating an output file name to store run settings infomation in. It is unique down to the second to keep the user from 
	// overwriting files (You just cannot save more than one file a second).
	time_t t = time(0); 
	struct tm * now = localtime( & t );
	int month = now->tm_mon + 1, day = now->tm_mday, curTimeHour = now->tm_hour, curTimeMin = now->tm_min, curTimeSec = now->tm_sec;
	stringstream smonth, sday, stimeHour, stimeMin, stimeSec;

	smonth << month;
	sday << day;
	stimeHour << curTimeHour;
	stimeMin << curTimeMin;
	stimeSec << curTimeSec;
	string monthday;
	
	// We are using <=9 below to keep the minutes and seconds in a two-digit format. 
	// For example, 2 seconds would be displayed as 02 seconds.
	if(curTimeMin <= 9)
	{
		if(curTimeSec <= 9) monthday = smonth.str() + "-" + sday.str() + "_" + stimeHour.str() + "-0" + stimeMin.str() + "-0" + stimeSec.str();
		else monthday = smonth.str() + "-" + sday.str() + "_" + stimeHour.str() + "-0" + stimeMin.str() + "-" + stimeSec.str();
	}
	else monthday = smonth.str() + "-" + sday.str() + "_" + stimeHour.str() + "-" + stimeMin.str() + "-" + stimeSec.str();

	string timeStamp = "Run_" + monthday;
	const char *directoryName = timeStamp.c_str();
	
	// Creating the diretory to hold the run settings.
	if(mkdir(directoryName
#ifndef _WIN32
		, 0777
#endif
		) == 0) // Create directory with appropriate permissions
	{
		printf("\n Directory '%s' created successfully.\n", directoryName);
	}
	else
	{
		printf("\n Error creating directory '%s'.\n", directoryName);
	}
	
	// Moving into the directory
	chdir(directoryName);
	
	// Copying all the nodes and muscle (with their properties) into this folder in the file named run.
	FILE *settingFile;
  	settingFile = fopen("run", "wb");
  	fwrite(&PulsePointNode, sizeof(int), 1, settingFile);
  	fwrite(&UpNode, sizeof(int), 1, settingFile);
  	fwrite(&FrontNode, sizeof(int), 1, settingFile);
  	fwrite(&NumberOfNodes, sizeof(int), 1, settingFile);
  	fwrite(&NumberOfMuscles, sizeof(int), 1, settingFile);
  	fwrite(&RadiusOfLeftAtrium, sizeof(double), 1, settingFile);
  	int linksPerNode = MUSCLES_PER_NODE;
  	fwrite(&linksPerNode, sizeof(int), 1, settingFile);
  	fwrite(Node, sizeof(nodeAttributesStructure), NumberOfNodes, settingFile);
  	fwrite(Muscle, sizeof(muscleAttributesStructure), NumberOfMuscles, settingFile);
	fclose(settingFile);
	
	//Copying the simulationSetup file into this directory so you will know how it was initally setup.
	FILE *fileIn;
	FILE *fileOut;
	long sizeOfFile;
  	char *buffer;

	//BASIC sim setup file
	fileIn = fopen("../../BasicSimulationSetup", "rb");

	if(fileIn == NULL)
	{
		printf("\n\n The basic simulationSetup file does not exist\n\n");
		exit(0);
	}

	// Finding the size of the BasicSimulationSetup file.
	fseek (fileIn , 0 , SEEK_END);
  	sizeOfFile = ftell(fileIn);
  	rewind (fileIn);
  	
  	// Creating a buffer to hold the BasicSimulationSetup file.
  	buffer = (char*)malloc(sizeof(char)*sizeOfFile);
  	fread (buffer, 1, sizeOfFile, fileIn);
	fileOut = fopen("BasicSimulationSetup", "wb");
	fwrite (buffer, 1, sizeOfFile, fileOut);
	fclose(fileIn);
	fclose(fileOut);
	free(buffer);


	//INTERMEDIATE sim setup file
	fileIn = fopen("../../IntermediateSimulationSetup", "rb");

	if(fileIn == NULL)
	{
		printf("\n\n The intermediate simulationSetup file does not exist\n\n");
		exit(0);
	}

	// Finding the size of the IntermediateSimulationSetup file.
	fseek (fileIn , 0 , SEEK_END);
  	sizeOfFile = ftell(fileIn);
  	rewind (fileIn);
  	
  	// Creating a buffer to hold the simulationSetup file.
  	buffer = (char*)malloc(sizeof(char)*sizeOfFile);
  	fread (buffer, 1, sizeOfFile, fileIn);
	fileOut = fopen("IntermediateSimulationSetup", "wb");
	fwrite (buffer, 1, sizeOfFile, fileOut);
	fclose(fileIn);
	fclose(fileOut);
	free(buffer);

	//ADVANCED sim setup file
	fileIn = fopen("../../AdvancedSimulationSetup", "rb");

	if(fileIn == NULL)
	{
		printf("\n\n The advanced simulationSetup file does not exist\n\n");
		exit(0);
	}

	// Finding the size of the AdvancedSimulationSetup file.
	fseek (fileIn , 0 , SEEK_END);
  	sizeOfFile = ftell(fileIn);
  	rewind (fileIn);
  	
  	// Creating a buffer to hold the AdvancedSimulationSetup file.
  	buffer = (char*)malloc(sizeof(char)*sizeOfFile);
  	fread (buffer, 1, sizeOfFile, fileIn);
	fileOut = fopen("AdvancedSimulationSetup", "wb");
	fwrite (buffer, 1, sizeOfFile, fileOut);
	fclose(fileIn);
	fclose(fileOut);
	free(buffer);
	
	// Making a readMe file to put any infomation about why you are saving this run.
	// Note: Edit the 'readMe' file in the saved run directory to add notes about this run.
	
	// Moving back to the SVT directory.
	chdir("../");
}

void saveState()
{
    // Copy latest data from GPU
    cudaMemcpy(Node, NodeGPU, NumberOfNodes * sizeof(nodeAttributesStructure), cudaMemcpyDeviceToHost);
    cudaMemcpy(Muscle, MuscleGPU, NumberOfMuscles * sizeof(muscleAttributesStructure), cudaMemcpyDeviceToHost);

    // Open file for writing (binary mode)
	// we're using a binary file (.bin) because it is faster and smaller than a text file, we can read and write the entire structs at once
	//rather than writing each variable one at a time, translating to and from text, and dealing with formatting
	//its also worth noting that any variable can be easily saved, so we can easily add anything to this
    FILE* file = fopen("simulation_state.bin", "wb");
    if (!file) 
	{
        printf("Error: Could not open file for saving state.\n");
        return;
    }

    // Save runtime value
    fwrite(&RunTime, sizeof(double), 1, file);
    
    // Save simulation timers and relevant state variables
	//this lets you save the mouse function you're in, I thought it might be useful for trigger placement, so I added it
    fwrite(&Simulation, sizeof(Simulation), 1, file);

    // Save node and muscle counts
    fwrite(&NumberOfNodes, sizeof(int), 1, file);
    fwrite(&NumberOfMuscles, sizeof(int), 1, file);

    // Save all nodes and muscles
    fwrite(Node, sizeof(nodeAttributesStructure), NumberOfNodes, file);
    fwrite(Muscle, sizeof(muscleAttributesStructure), NumberOfMuscles, file);

    fclose(file);
    //printf("Simulation state saved at runtime: %.2f ms\n", RunTime);
}

void loadState()
{
    // Open file for reading (binary mode)
    FILE* file = fopen("simulation_state.bin", "rb");
    if (!file) 
	{
        printf("Error: Could not open file for loading state.\n");
        return;
    }

    // Load runtime value
    fread(&RunTime, sizeof(double), 1, file);
    
    // Load simulation timers and relevant state variables
    fread(&Simulation, sizeof(Simulation), 1, file);

    // Load node and muscle counts
    int nNodes, nMuscles;
    fread(&nNodes, sizeof(int), 1, file);
    fread(&nMuscles, sizeof(int), 1, file);

    // Sanity check
    if (nNodes != NumberOfNodes || nMuscles != NumberOfMuscles) 
    {
        printf("Error: Node/Muscle count mismatch. State not loaded.\n");
        fclose(file);
        return;
    }

    // Load all nodes and muscles
    fread(Node, sizeof(nodeAttributesStructure), NumberOfNodes, file);
    fread(Muscle, sizeof(muscleAttributesStructure), NumberOfMuscles, file);

    fclose(file);

    // Copy loaded data back to GPU
    cudaMemcpy(NodeGPU, Node, NumberOfNodes * sizeof(nodeAttributesStructure), cudaMemcpyHostToDevice);
    cudaMemcpy(MuscleGPU, Muscle, NumberOfMuscles * sizeof(muscleAttributesStructure), cudaMemcpyHostToDevice);

    drawPicture();
	Simulation.isPaused = true; // Pause the simulation after loading state
    //printf("Simulation state loaded at runtime: %.2f ms\n", RunTime);
}

void findNodes()
{
	copyNodesFromGPU();

	//Reset previous nodes if they exist
	if (Simulation.frontNodeIndex >= 0 && Simulation.topNodeIndex >= 0) //if the front and top node indices are valid
	
	{
		// Reset front node based on ablation status
		if (Node[Simulation.frontNodeIndex].isAblated) 
		{
			Node[Simulation.frontNodeIndex].isDrawNode = true; // Keep it visible, set to white
			Node[Simulation.frontNodeIndex].color.x = 1.0f;
			Node[Simulation.frontNodeIndex].color.y = 1.0f;
			Node[Simulation.frontNodeIndex].color.z = 1.0f;
		} 
		else 
		{
			Node[Simulation.frontNodeIndex].isDrawNode = false; //back to default color
			Node[Simulation.frontNodeIndex].color.x = 0.0f;
			Node[Simulation.frontNodeIndex].color.y = 1.0f;
			Node[Simulation.frontNodeIndex].color.z = 0.0f;
		}

		// Reset top node based on ablation status
		if (Node[Simulation.topNodeIndex].isAblated) 
		{
			Node[Simulation.topNodeIndex].isDrawNode = true; // Keep it visible, set color to white
			Node[Simulation.topNodeIndex].color.x = 1.0f;
			Node[Simulation.topNodeIndex].color.y = 1.0f;
			Node[Simulation.topNodeIndex].color.z = 1.0f;
		} 
		else 
		{
			Node[Simulation.topNodeIndex].isDrawNode = false; //back to default color
			Node[Simulation.topNodeIndex].color.x = 0.0f;
			Node[Simulation.topNodeIndex].color.y = 1.0f;
			Node[Simulation.topNodeIndex].color.z = 0.0f;
		}
	}

	//give bad values to the indices so we know they are not valid unless they are made valid again
	float maxZ = -10000.0;
	float maxY = -10000.0;
	int indexZ = -1;
	int indexY = -1;
	
	// Loop through all nodes, checking for the max Z and Y values
	for(int i = 0; i < NumberOfNodes; i++)
	{
		if(maxZ < Node[i].position.z) 
		{
			maxZ = Node[i].position.z;
			indexZ = i;
		}
		
		if(maxY < Node[i].position.y) 
		{
			maxY = Node[i].position.y;
			indexY = i;
		}
	}
	
	//set the colors of the nodes to blue and purple, respectively
	Node[indexZ].isDrawNode = true; // Set the front node to be drawn as blue
	Node[indexZ].color.x = 0.0;
	Node[indexZ].color.y = 0.0;
	Node[indexZ].color.z = 1.0;
	
	Node[indexY].isDrawNode = true; // Set the top node to be drawn as purple
	Node[indexY].color.x = 1.0;
	Node[indexY].color.y = 0.0;
	Node[indexY].color.z = 1.0;
	
	// Store indices for persistent display
	Simulation.frontNodeIndex = indexZ;
	Simulation.topNodeIndex = indexY;
	Simulation.nodesFound = true;
	
	drawPicture(); // Redraw the picture to show the new colors
	copyNodesToGPU(); // Copy the updated nodes back to GPU (since the color changed)
}

/*
 This function directs the action that needs to be taken if a user hits a key on the key board.
 The terminal screen lists out all the keys and what they will do.
*/
void KeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	//See if GUI wants this event (Prevents keys from being registered when doing things like typing in a text box)
	    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
        return;


    // Only process key press events, not releases or repeats

	if (action != GLFW_PRESS) return;

	// Check for specific key presses
	switch (key)
	{
		case GLFW_KEY_ESCAPE: // Escape key to exit
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;

		case GLFW_KEY_F1: // F1 key to toggle run/pause
		case GLFW_KEY_R: // r/R key to toggle run/pause
			if(Simulation.isPaused)
			{
				Simulation.isPaused = false;
			}
			else
			{
				Simulation.isPaused = true;
			}
			break;

		case GLFW_KEY_F2: // F2 key to draw only half of the nodes
			if(Simulation.DrawFrontHalfFlag)
			{
				Simulation.DrawFrontHalfFlag = false;
			}
			else
			{
				Simulation.DrawFrontHalfFlag = true;
			}
			drawPicture();
			break;

		case GLFW_KEY_F3: //show nodes 0 = none 1 = half 2 = all
			if(Simulation.DrawNodesFlag == 0)
			{
				Simulation.DrawNodesFlag = 1;
			}
			else if(Simulation.DrawNodesFlag == 1)
			{
				Simulation.DrawNodesFlag = 2;
			}
			else
			{
				Simulation.DrawNodesFlag = 0;
			}
			drawPicture();
			break;

		case GLFW_KEY_F4: // Toggle movie recording
			if(Simulation.isRecording)
			{
				movieOff();
			}
			else
			{
				movieOn();
			}
			break;

		case GLFW_KEY_F5: // Take a screenshot
			screenShot();
			break;

		case GLFW_KEY_F6: // Save settings
			saveSettings();
			break;

		case GLFW_KEY_F7: // Toggle ablate mode
			if(Simulation.isInAblateMode)
			{
				mouseFunctionsOff();
			}
			else
			{
				mouseAblateMode();
			}
			break;
		
		case GLFW_KEY_F8: // Toggle ectopic beat mode
			if(Simulation.isInEctopicBeatMode)
			{
				mouseFunctionsOff();
			}
			else
			{
				mouseEctopicBeatMode();
			}
			break;

		case GLFW_KEY_F9: // Toggle ectopic event mode
			if(Simulation.isInEctopicEventMode)
			{
				mouseFunctionsOff();
			}
			else
			{
				mouseEctopicEventMode();
			}
			break;

		case GLFW_KEY_F10: // Toggle adjust muscle area mode
			if(Simulation.isInAdjustMuscleAreaMode)
			{
				mouseFunctionsOff();
			}
			else
			{
				mouseAdjustMusclesAreaMode();
			}
			break;
		
		case GLFW_KEY_F11: // Toggle adjust muscle line mode
			if(Simulation.isInAdjustMuscleLineMode)
			{
				mouseFunctionsOff();
			}
			else
			{
				mouseAdjustMusclesLineMode();
			}
			break;

		case GLFW_KEY_F12: // Toggle identify node mode
			if(Simulation.isInFindNodeMode)
			{
				mouseFunctionsOff();
			}
			else
			{
				mouseIdentifyNodeMode();
			}
			break;

		// ALT + q to turn mouse functions off
		case GLFW_KEY_Q:
			if (mods & GLFW_MOD_ALT)
			{
				mouseFunctionsOff();
			}
			break;

		//view toggles -- follows numpad format and matches with GUI
		//kp is numpad keys, so need 2 cases for each
		
		case GLFW_KEY_7: // PA View
		case GLFW_KEY_KP_7:
			setView(4); 
			copyNodesToGPU(); 
			drawPicture();
			break;
			
		case GLFW_KEY_8: // AP View
		case GLFW_KEY_KP_8:
			setView(2);
			copyNodesToGPU();
			drawPicture();
			break;
        
		case GLFW_KEY_9: // Reference view (top-right)
        case GLFW_KEY_KP_9: 
            setView(6);
            copyNodesToGPU();
            drawPicture();
            break;
           
		case GLFW_KEY_4: // LAO view (middle-left)
        case GLFW_KEY_KP_4: 
            setView(1);
            copyNodesToGPU();
            drawPicture();
            break;
        case GLFW_KEY_5: // RAO view (middle)
        case GLFW_KEY_KP_5: 
            setView(3);
            copyNodesToGPU();
            drawPicture();
            break;
        
		case GLFW_KEY_6: // LL view (middle-right)
        case GLFW_KEY_KP_6: 
            setView(7);
            copyNodesToGPU();
            drawPicture();
            break;
        
		case GLFW_KEY_1: // RL view (bottom-left)
        case GLFW_KEY_KP_1:
            setView(9);
            copyNodesToGPU();
            drawPicture();
            break;
        
		case GLFW_KEY_2: // Superior view (bottom-middle)
        case GLFW_KEY_KP_2:
            setView(8);
            copyNodesToGPU();
            drawPicture();
            break;
        
		case GLFW_KEY_3: // Inferior view (bottom-right)
        case GLFW_KEY_KP_3:
            setView(5);
            copyNodesToGPU();
            drawPicture();
            break;
		
		/* Ortho/frustum needs to be fixed */
		// case GLFW_KEY_0: // Toggle orthogonal/frustum view
		// case GLFW_KEY_KP_0:
		// 	if (Simulation.ViewFlag == 0)
        //     {
        //         Simulation.ViewFlag = 1;
        //         frustumView();
        //     }
        //     else if (Simulation.ViewFlag == 1)
        //     {
        //         Simulation.ViewFlag = 0;
        //         orthogonalView();
        //     }

		// Might make this a held key pending feedback
		case GLFW_KEY_KP_SUBTRACT: //decrease selection radius
		case GLFW_KEY_MINUS:
		HitMultiplier -= 0.01;
		if(HitMultiplier < 0.01) HitMultiplier = 0.01;
		break;

		case GLFW_KEY_KP_ADD: //increase selection radius
		case GLFW_KEY_EQUAL:
		HitMultiplier += 0.025;
		if(HitMultiplier > 0.5) HitMultiplier = 0.5;
		break;

		case GLFW_KEY_LEFT_BRACKET: //decrease beat period
			Node[PulsePointNode].beatPeriod -= 10;
            if(Node[PulsePointNode].beatPeriod < 0) // Prevent negative beat period 
			{
                Node[PulsePointNode].beatPeriod = 0;
            }
			copyNodesToGPU();
			break;

		case GLFW_KEY_RIGHT_BRACKET: //increase beat period
			Node[PulsePointNode].beatPeriod += 10;
			if(Node[PulsePointNode].beatPeriod > 10000) // Prevent excessively large beat period
			{
				Node[PulsePointNode].beatPeriod = 10000;
			}
			copyNodesToGPU();
			break;

		case GLFW_KEY_SEMICOLON: //decrease DrawRate
			DrawRate -= 50;
			if(DrawRate < 100) DrawRate = 100;
			break;

		case GLFW_KEY_APOSTROPHE: //increase DrawRate
			DrawRate += 50;
			if(DrawRate > 5000) DrawRate = 5000;
			break;

		case GLFW_KEY_F: //Alt + f to find nodes
			if (mods & GLFW_MOD_ALT)
			{
				findNodes();
			}
			break;

		case GLFW_KEY_S: // Ctrl + s to save state
			if (mods & GLFW_MOD_CONTROL)
			{
				saveState();
			}
			break;
		
		case GLFW_KEY_Z: // Ctrl + z to load state
			if (mods & GLFW_MOD_CONTROL)
			{
				loadState();
			}
			break;
		
		case GLFW_KEY_H: // Ctrl + h to collapse/expand GUI
		if (mods & GLFW_MOD_CONTROL)
		{
			Simulation.guiCollapsed = !Simulation.guiCollapsed;
		}
		break;

		default: // For any other key, do nothing
			break;

	}
    
    
}

/*
	This function will process held keys.
	Since GLFW will not allow us to have 2 key call backs and seems to force us to use either presses or holds
	we will use this function to process held keys.

	We will need to consider all early exit cases for keys that don't need to be held and will need to handle shift keys in a different way

*/
void keyHeld(GLFWwindow* window)
{
	// Check if any movement keys (or keys we want to work if held) are pressed
    bool validKey = 
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;

	//early exit if no valid key is pressed
	if (!validKey) return;

	bool shiftHeld = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
						glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

	// Copy nodes from GPU once per frame
    copyNodesFromGPU();

	float dAngle = 0.01;
	float zoom = 0.01*RadiusOfLeftAtrium;
	float temp;
	float4 lookVector;
	float d;
	float4 centerOfMass;
	
	//copyNodesMusclesFromGPU();
	
	lookVector.x = CenterX - EyeX;
	lookVector.y = CenterY - EyeY;
	lookVector.z = CenterZ - EyeZ;
	d = sqrt(lookVector.x*lookVector.x + lookVector.y*lookVector.y + lookVector.z*lookVector.z);
	
	if(d < 0.00001)
	{
		printf("\n lookVector is too small\n");
		printf("\n Good Bye\n");
		exit(0);
	}
	else
	{
		lookVector.x /= d;
		lookVector.y /= d;
		lookVector.z /= d;
	}

	// WASD movement keys
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !shiftHeld))   // Rotate counterclockwise on the x-axis
	{
		centerOfMass = findCenterOfMass();
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Node[i].position.x -= centerOfMass.x;
			Node[i].position.y -= centerOfMass.y;
			Node[i].position.z -= centerOfMass.z;
			temp = cos(dAngle)*Node[i].position.y - sin(dAngle)*Node[i].position.z;
			Node[i].position.z  = sin(dAngle)*Node[i].position.y + cos(dAngle)*Node[i].position.z;
			Node[i].position.y  = temp;
			Node[i].position.x += centerOfMass.x;
			Node[i].position.y += centerOfMass.y;
			Node[i].position.z += centerOfMass.z;
		}
		AngleOfSimulation.x += dAngle;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS  || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !shiftHeld))   // Rotate clockwise on the y-axis
	{
		centerOfMass = findCenterOfMass();
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Node[i].position.x -= centerOfMass.x;
			Node[i].position.y -= centerOfMass.y;
			Node[i].position.z -= centerOfMass.z;
			temp = cos(dAngle)*Node[i].position.x + sin(dAngle)*Node[i].position.z;
			Node[i].position.z  = -sin(dAngle)*Node[i].position.x + cos(dAngle)*Node[i].position.z;
			Node[i].position.x  = temp;
			Node[i].position.x += centerOfMass.x;
			Node[i].position.y += centerOfMass.y;
			Node[i].position.z += centerOfMass.z;
		}
		AngleOfSimulation.y += dAngle;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS  || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !shiftHeld))  // Rotate clockwise on the x-axis
	{
		centerOfMass = findCenterOfMass();
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Node[i].position.x -= centerOfMass.x;
			Node[i].position.y -= centerOfMass.y;
			Node[i].position.z -= centerOfMass.z;
			temp = cos(-dAngle)*Node[i].position.y - sin(-dAngle)*Node[i].position.z;
			Node[i].position.z  = sin(-dAngle)*Node[i].position.y + cos(-dAngle)*Node[i].position.z;
			Node[i].position.y  = temp; 
			Node[i].position.x += centerOfMass.x;
			Node[i].position.y += centerOfMass.y;
			Node[i].position.z += centerOfMass.z;
		}
		AngleOfSimulation.x -= dAngle;
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS  || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !shiftHeld))  // Rotate counterclockwise on the y-axis
	{
		centerOfMass = findCenterOfMass();
		for(int i = 0; i < NumberOfNodes; i++)
		{
			Node[i].position.x -= centerOfMass.x;
			Node[i].position.y -= centerOfMass.y;
			Node[i].position.z -= centerOfMass.z;
			temp =  cos(-dAngle)*Node[i].position.x + sin(-dAngle)*Node[i].position.z;
			Node[i].position.z  = -sin(-dAngle)*Node[i].position.x + cos(-dAngle)*Node[i].position.z;
			Node[i].position.x  = temp;
			Node[i].position.x += centerOfMass.x;
			Node[i].position.y += centerOfMass.y;
			Node[i].position.z += centerOfMass.z;
		}
		AngleOfSimulation.y -= dAngle;
	}
	

	
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) 
	{
		if(shiftHeld)  // Uppercase Z - Rotate clockwise on the z-axis
		{
			centerOfMass = findCenterOfMass();
			for(int i = 0; i < NumberOfNodes; i++)
			{
				Node[i].position.x -= centerOfMass.x;
				Node[i].position.y -= centerOfMass.y;
				Node[i].position.z -= centerOfMass.z;
				temp = cos(-dAngle)*Node[i].position.x - sin(-dAngle)*Node[i].position.y;
				Node[i].position.y  = sin(-dAngle)*Node[i].position.x + cos(-dAngle)*Node[i].position.y;
				Node[i].position.x  = temp;
				Node[i].position.x += centerOfMass.x;
				Node[i].position.y += centerOfMass.y;
				Node[i].position.z += centerOfMass.z;
			}
			AngleOfSimulation.z -= dAngle;
		}
		else  // Lowercase z - Rotate counterclockwise on the z-axis
		{
			centerOfMass = findCenterOfMass();
			for(int i = 0; i < NumberOfNodes; i++)
			{
				Node[i].position.x -= centerOfMass.x;
				Node[i].position.y -= centerOfMass.y;
				Node[i].position.z -= centerOfMass.z;
				temp = cos(dAngle)*Node[i].position.x - sin(dAngle)*Node[i].position.y;
				Node[i].position.y  = sin(dAngle)*Node[i].position.x + cos(dAngle)*Node[i].position.y;
				Node[i].position.x  = temp;
				Node[i].position.x += centerOfMass.x;
				Node[i].position.y += centerOfMass.y;
				Node[i].position.z += centerOfMass.z;
			}
			AngleOfSimulation.z += dAngle;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) 
	{
		if(shiftHeld)  // Uppercase E - Zoom out
		{
			for(int i = 0; i < NumberOfNodes; i++)
			{
				Node[i].position.x += zoom*lookVector.x;
				Node[i].position.y += zoom*lookVector.y;
				Node[i].position.z += zoom*lookVector.z;
			}
			CenterOfSimulation.x += zoom*lookVector.x;
			CenterOfSimulation.y += zoom*lookVector.y;
			CenterOfSimulation.z += zoom*lookVector.z;
		}
		else  // Lowercase e - Zoom in
		{
			for(int i = 0; i < NumberOfNodes; i++)
			{
				Node[i].position.x -= zoom*lookVector.x;
				Node[i].position.y -= zoom*lookVector.y;
				Node[i].position.z -= zoom*lookVector.z;
			}
			CenterOfSimulation.x -= zoom*lookVector.x;
			CenterOfSimulation.y -= zoom*lookVector.y;
			CenterOfSimulation.z -= zoom*lookVector.z;
		}
	}

	if (shiftHeld) 
	{
		// Shift + Left/Right for Z-axis rotation (same as z/Z)
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
		{
			// Rotate clockwise on the z-axis (same as uppercase Z)
			centerOfMass = findCenterOfMass();
			for(int i = 0; i < NumberOfNodes; i++) 
			{
				Node[i].position.x -= centerOfMass.x;
				Node[i].position.y -= centerOfMass.y;
				Node[i].position.z -= centerOfMass.z;
				temp = cos(-dAngle)*Node[i].position.x - sin(-dAngle)*Node[i].position.y;
				Node[i].position.y = sin(-dAngle)*Node[i].position.x + cos(-dAngle)*Node[i].position.y;
				Node[i].position.x = temp;
				Node[i].position.x += centerOfMass.x;
				Node[i].position.y += centerOfMass.y;
				Node[i].position.z += centerOfMass.z;
			}
			AngleOfSimulation.z -= dAngle;
		}
		
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
		{
			// Rotate counterclockwise on the z-axis (same as lowercase z)
			centerOfMass = findCenterOfMass();
			for(int i = 0; i < NumberOfNodes; i++) 
			{
				Node[i].position.x -= centerOfMass.x;
				Node[i].position.y -= centerOfMass.y;
				Node[i].position.z -= centerOfMass.z;
				temp = cos(dAngle)*Node[i].position.x - sin(dAngle)*Node[i].position.y;
				Node[i].position.y = sin(dAngle)*Node[i].position.x + cos(dAngle)*Node[i].position.y;
				Node[i].position.x = temp;
				Node[i].position.x += centerOfMass.x;
				Node[i].position.y += centerOfMass.y;
				Node[i].position.z += centerOfMass.z;
			}
			AngleOfSimulation.z += dAngle;
		}
		
		// Shift + Up/Down for zooming (same as e/E)
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
		{
			// Zoom in (same as lowercase e)
			for(int i = 0; i < NumberOfNodes; i++) 
			{
				Node[i].position.x -= zoom*lookVector.x;
				Node[i].position.y -= zoom*lookVector.y;
				Node[i].position.z -= zoom*lookVector.z;
			}
			CenterOfSimulation.x -= zoom*lookVector.x;
			CenterOfSimulation.y -= zoom*lookVector.y;
			CenterOfSimulation.z -= zoom*lookVector.z;
		}
		
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
		{
			// Zoom out (same as uppercase E)
			for(int i = 0; i < NumberOfNodes; i++) 
			{
				Node[i].position.x += zoom*lookVector.x;
				Node[i].position.y += zoom*lookVector.y;
				Node[i].position.z += zoom*lookVector.z;
			}
			CenterOfSimulation.x += zoom*lookVector.x;
			CenterOfSimulation.y += zoom*lookVector.y;
			CenterOfSimulation.z += zoom*lookVector.z;
		}
	}

	drawPicture(); // Redraw the picture after all the changes
	copyNodesToGPU(); // Copy the modified nodes back to the GPU

}
/*
 This function is called when the mouse moves without any button pressed.
 x and y are the current mouse coordinates.
 x come in as (0, XWindowSize) and y comes in as (0, YWindowSize). 
 We translates them to MouseX (-1, 1) and MouseY (-1, 1) to corospond to the openGL window size.
 We then use MouseX and MouseY to determine where the mouse is in the simulation.
*/
void mousePassiveMotionCallback(GLFWwindow* window, double x, double y)
{
	// Get ImGui IO to check if mouse is over ImGui windows
    ImGuiIO& io = ImGui::GetIO();

	//Show cursor when highlighting over IMGUI elements
	if (Simulation.isInMouseFunctionMode)
	{
		//Uncomment this to have the cursor show when it hovers the GUI in mouse function mode
		if (io.WantCaptureMouse)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			return; // If ImGui is capturing the mouse, do not process further
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		
	}
	
	float sensitivityMultiplier = 1.2; // Sensitivity multiplier for mouse movement
	MouseX = ( 2.0*x/XWindowSize - 1.0)*RadiusOfLeftAtrium *sensitivityMultiplier;
	MouseY = (-2.0*y/YWindowSize + 1.0)*RadiusOfLeftAtrium *sensitivityMultiplier;
}

/*
 This function does an action based on the mode the viewer is in and which mouse button the user pressed.
*/
void myMouse(GLFWwindow* window, int button, int action, int mods)
{	

	//Add this if we want the GUI to only accept GUI handling until you ckick off of it
    // Get ImGui IO to check if it's capturing input
    ImGuiIO& io = ImGui::GetIO();
    
    // If ImGui is handling this mouse event, return
    if (io.WantCaptureMouse) return;
	
	float d, dx, dy, dz;
	float hit;
	int muscleId;
	
	if(action == GLFW_PRESS)
	{
		copyNodesMusclesFromGPU();
		hit = HitMultiplier*RadiusOfLeftAtrium;
		
		if(button == GLFW_MOUSE_BUTTON_LEFT)
		{	
			if(Simulation.isInAdjustMuscleLineMode)
			{
				// Finding the two closest nodes to the mouse.
				int nodeId1 = -1;
				int nodeId2 = -1;
				int connectingMuscle = -1;
				int test = -1;
				float minDistance = 2.0*RadiusOfLeftAtrium;
				for(int i = 0; i < NumberOfNodes; i++)
				{
					dx = MouseX - Node[i].position.x;
					dy = MouseY - Node[i].position.y;
					dz = MouseZ - Node[i].position.z;
					d = sqrt(dx*dx + dy*dy + dz*dz);
					if(d < minDistance)
					{
						minDistance = d;
						nodeId2 = nodeId1;
						nodeId1 = i;
					}
				}
				
				// If for some reason two nodes were not found. Not sure how this could
				// happen, but just to be safe we put a check in here.
				if(nodeId2 == -1)
				{
					printf("\n Two nodes were not found try again.\n");
					printf("\n MouseZ = %lf.\n", MouseZ);
				}
				// We got the two closest nodes to the mouse. Now see if there is a muscle that
				// connects these two nodes. If there is a connecting muscle, adjust it.
				else
				{
					if(!Node[nodeId1].isAblated)
					{
						Node[nodeId1].color.x = 1.0;
						Node[nodeId1].color.y = 0.0;
						Node[nodeId1].color.z = 1.0;
						Node[nodeId1].isDrawNode = true;
					}
					
					if(!Node[nodeId2].isAblated)
					{
						Node[nodeId2].color.x = 1.0;
						Node[nodeId2].color.y = 0.0;
						Node[nodeId2].color.z = 1.0;
						Node[nodeId2].isDrawNode = true;
					}
					
					for(int i = 0; i < MUSCLES_PER_NODE; i++) // Spinnning through muscles on node 1.
					{
						muscleId = Node[nodeId1].muscle[i]; 
						if(muscleId != -1)
						{
							for(int j = 0; j < MUSCLES_PER_NODE; j++) // Spinnning through muscles on node 2.
							{
								test = Node[nodeId2].muscle[j];
								if(muscleId == test) // Checking to see if we get a match.
								{
									connectingMuscle = muscleId;
								}
							}
						}
					}
					if(connectingMuscle == -1)
					{
						printf("\n No connecting muscle was found try again.\n");
					}
					else
					{
						muscleId = connectingMuscle;
						Muscle[muscleId].refractoryPeriod = BaseMuscleRefractoryPeriod*RefractoryPeriodAdjustmentMultiplier;
						Muscle[muscleId].conductionVelocity = BaseMuscleConductionVelocity*MuscleConductionVelocityAdjustmentMultiplier;
						Muscle[muscleId].conductionDuration = Muscle[muscleId].naturalLength/Muscle[muscleId].conductionVelocity;
						Muscle[muscleId].color.x = 1.0;
						Muscle[muscleId].color.y = 0.0;
						Muscle[muscleId].color.z = 1.0;
						Muscle[muscleId].color.w = 0.0;
						
						checkMuscle(muscleId);		
					}
				}
			}
			else
			{
				for(int i = 0; i < NumberOfNodes; i++)
				{
					dx = MouseX - Node[i].position.x;
					dy = MouseY - Node[i].position.y;
					dz = MouseZ - Node[i].position.z;
					
					if(sqrt(dx*dx + dy*dy + dz*dz) < hit)
					{
						if(Simulation.isInAblateMode)
						{
							Node[i].isAblated = true;
							Node[i].isDrawNode = true;
							Node[i].color.x = 1.0;
							Node[i].color.y = 1.0;
							Node[i].color.z = 1.0;
						}
						
						if(Simulation.isInEctopicBeatMode)
						{
							Simulation.isPaused = true;
							// printf("\n Node number = %d", i);
							setEctopicBeat(i);
						}
						
						if(Simulation.isInAdjustMuscleAreaMode)
						{
							for(int j = 0; j < MUSCLES_PER_NODE; j++)
							{
								muscleId = Node[i].muscle[j];
								if(muscleId != -1)
								{
									// This sets the muscle to the base value then adjusts it. 
									Muscle[muscleId].refractoryPeriod = BaseMuscleRefractoryPeriod*RefractoryPeriodAdjustmentMultiplier;
									Muscle[muscleId].conductionVelocity = BaseMuscleConductionVelocity*MuscleConductionVelocityAdjustmentMultiplier;
									
									// This adjusts the muscle based on its current value.
									//Muscle[muscleId].refractoryPeriod *= RefractoryPeriodAdjustmentMultiplier;
									//Muscle[muscleId].conductionVelocity *= MuscleConductionVelocityAdjustmentMultiplier;
									
									Muscle[muscleId].conductionDuration = Muscle[muscleId].naturalLength/Muscle[muscleId].conductionVelocity;
									Muscle[muscleId].color.x = 1.0;
									Muscle[muscleId].color.y = 0.0;
									Muscle[muscleId].color.z = 1.0;
									Muscle[muscleId].color.w = 0.0;
									
									checkMuscle(muscleId);
								}
							}
							
							Node[i].isDrawNode = true;
							if(!Node[i].isAblated) // If it is not ablated color it.
							{
								Node[i].color.x = 0.8;
								Node[i].color.y = 0.3;
								Node[i].color.z = 1.0;
							}
						}
						
						if(Simulation.isInEctopicEventMode)
						{
							cudaMemcpy( Node, NodeGPU, NumberOfNodes*sizeof(nodeAttributesStructure), cudaMemcpyDeviceToHost);
							cudaErrorCheck(__FILE__, __LINE__);
							
							Node[i].isFiring = true; // Setting the ith node to fire the next time in the next time step.

							//Create a pink point sprite at the node
							Node[i].color.x = 0.996;
							Node[i].color.y = 0.242;
							Node[i].color.z = 0.637;
							Node[i].isDrawNode = true;
							
							cudaMemcpy( NodeGPU, Node, NumberOfNodes*sizeof(nodeAttributesStructure), cudaMemcpyHostToDevice );
							cudaErrorCheck(__FILE__, __LINE__);
						}
						
						if(Simulation.isInFindNodeMode)
						{
							Node[i].isDrawNode = true;
							Node[i].color.x = 1.0;
							Node[i].color.y = 0.0;
							Node[i].color.z = 1.0;
							// printf("\n Node number = %d", i);
						}
					}
				}
			}
		}
		else if(button == GLFW_MOUSE_BUTTON_RIGHT) // Right Mouse button down
		{
			if(Simulation.isInAdjustMuscleLineMode)
			{
				// Finding the two closest nodes to the mouse.
				int nodeId1 = -1;
				int nodeId2 = -1;
				int connectingMuscle = -1;
				int test = -1;
				float minDistance = 2.0*RadiusOfLeftAtrium;
				for(int i = 0; i < NumberOfNodes; i++)
				{
					dx = MouseX - Node[i].position.x;
					dy = MouseY - Node[i].position.y;
					dz = MouseZ - Node[i].position.z;
					d = sqrt(dx*dx + dy*dy + dz*dz);
					if(d < minDistance)
					{
						minDistance = d;
						nodeId2 = nodeId1;
						nodeId1 = i;
					}
				}
				
				// If for some reason two nodes were not found. Not sure how this could
				// happen, but just to be safe we put a check in here.
				if(nodeId2 == -1)
				{
					printf("\n Two nodes were not found try again.\n");
					printf("\n MouseZ = %lf.\n", MouseZ);
				}
				// We got the two closest nodes to the mouse. Now see if there is a muscle that
				// connects these two nodes. If there is a connecting muscle, adjust it.
				else
				{
					if(!Node[nodeId1].isAblated)
					{
						Node[nodeId1].color.x = 0.0;
						Node[nodeId1].color.y = 1.0;
						Node[nodeId1].color.z = 0.0;
						Node[nodeId1].isDrawNode = false;
					}
					
					if(!Node[nodeId2].isAblated)
					{
						Node[nodeId2].color.x = 0.0;
						Node[nodeId2].color.y = 1.0;
						Node[nodeId2].color.z = 0.0;
						Node[nodeId2].isDrawNode = false;
					}
					
					for(int i = 0; i < MUSCLES_PER_NODE; i++) // Spinnning through muscles on node 1.
					{
						muscleId = Node[nodeId1].muscle[i]; 
						if(muscleId != -1)
						{
							for(int j = 0; j < MUSCLES_PER_NODE; j++) // Spinnning through muscles on node 2.
							{
								test = Node[nodeId2].muscle[j];
								if(muscleId == test) // Checking to see if we get a match.
								{
									connectingMuscle = muscleId;
								}
							}
						}
					}
					if(connectingMuscle == -1)
					{
						printf("\n No connecting muscle was found try again.\n");
					}
					else
					{
						muscleId = connectingMuscle;
						Muscle[muscleId].refractoryPeriod = BaseMuscleRefractoryPeriod;
						Muscle[muscleId].conductionVelocity = BaseMuscleConductionVelocity;
						Muscle[muscleId].conductionDuration = Muscle[muscleId].naturalLength/Muscle[muscleId].conductionVelocity;
						Muscle[muscleId].color.x = 0.0;
						Muscle[muscleId].color.y = 1.0;
						Muscle[muscleId].color.z = 0.0;
						Muscle[muscleId].color.w = 0.0;
						// Turning the muscle back on if it was disabled.
						Muscle[muscleId].isEnabled = true;
						
						checkMuscle(muscleId);		
					}
				}
			}
			else
			{
				for(int i = 0; i < NumberOfNodes; i++)
				{
					dx = MouseX - Node[i].position.x;
					dy = MouseY - Node[i].position.y;
					dz = MouseZ - Node[i].position.z;
					if(sqrt(dx*dx + dy*dy + dz*dz) < hit)
					{
						if(Simulation.isInAblateMode)
						{
							Node[i].isAblated = false;
							Node[i].isDrawNode = false;
							Node[i].color.x = 0.0;
							Node[i].color.y = 1.0;
							Node[i].color.z = 0.0;
						}
						
						if(Simulation.isInAdjustMuscleAreaMode)
						{
							for(int j = 0; j < MUSCLES_PER_NODE; j++)
							{
								muscleId = Node[i].muscle[j];
								if(muscleId != -1)
								{
									Muscle[muscleId].refractoryPeriod = BaseMuscleRefractoryPeriod;
									Muscle[muscleId].conductionVelocity = BaseMuscleConductionVelocity;
									Muscle[muscleId].conductionDuration = Muscle[muscleId].naturalLength/Muscle[muscleId].conductionVelocity;
									Muscle[muscleId].color.x = 0.0;
									Muscle[muscleId].color.y = 1.0;
									Muscle[muscleId].color.z = 0.0;
									Muscle[muscleId].color.w = 0.0;
									
									// Turning the muscle back on if it was disabled.
									Muscle[muscleId].isEnabled = true;
									
									// Checking to see if the muscle needs to be killed.
									checkMuscle(muscleId);
								}
							}
							
							Node[i].isDrawNode = true;
							if(!Node[i].isAblated) // If it is not ablated color it.
							{
								Node[i].color.x = 0.0;
								Node[i].color.y = 1.0;
								Node[i].color.z = 0.0;
							}
						}

						//Reset ectopic trigger colors
						if(Simulation.isInEctopicEventMode)
						{
							Node[i].color.x = 0.0;
							Node[i].color.y = 1.0;
							Node[i].color.z = 0.0;
						}
					}
				}
			}
		}
		else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			if(ScrollSpeedToggle == 0)
			{
				ScrollSpeedToggle = 1;
				ScrollSpeed = 1.0;
				// printf("\n speed = %f\n", ScrollSpeed);
			}
			else
			{
				ScrollSpeedToggle = 0;
				ScrollSpeed = 0.1;
				// printf("\n speed = %f\n", ScrollSpeed);
			}
			
		}
		drawPicture();
		copyNodesMusclesToGPU();
		//printf("\nSNx = %f SNy = %f SNz = %f\n", NodePosition[0].x, NodePosition[0].y, NodePosition[0].z);
	}
	
}

void scrollWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset > 0) // Scroll up
    {
        MouseZ -= ScrollSpeed;
    }
    else if(yoffset < 0) // Scroll down
    {
        MouseZ += ScrollSpeed;
    }
    // printf("MouseZ = %f\n", MouseZ);
    drawPicture();
}
