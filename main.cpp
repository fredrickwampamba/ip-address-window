#include <winsock2.h>
#include <windows.h>

#include <iphlpapi.h>
#include <stdio.h>
using namespace std;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(0,"WindowClass","MY IP ADDRESS",WS_VISIBLE,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		300, /* width */
		100, /* height */
		HWND_DESKTOP,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	DWORD Err;

    PFIXED_INFO pFixedInfo;
    DWORD FixedInfoSize = 0;

    PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
    DWORD AdapterInfoSize;
    PIP_ADDR_STRING pAddrStr;

    UINT i;

    struct tm newtime;
    char buffer[32];    
    errno_t error;

    //
    // Get the main IP configuration information for this machine using a FIXED_INFO structure
    //
    if ((Err = GetNetworkParams(NULL, &FixedInfoSize)) != 0)
    {
        if (Err != ERROR_BUFFER_OVERFLOW)
        {
            printf("GetNetworkParams sizing failed with error %d\n", Err);
            return 0;
        }
    }

    // Allocate memory from sizing information
    if ((pFixedInfo = (PFIXED_INFO) GlobalAlloc(GPTR, FixedInfoSize)) == NULL)
    {
        printf("Memory allocation error\n");
        return 0;
    }

    if ((Err = GetNetworkParams(pFixedInfo, &FixedInfoSize)) == 0)
    {
        printf("\tHost Name . . . . . . . . . : %s\n", pFixedInfo->HostName);
        printf("\tDNS Servers . . . . . . . . : %s\n", pFixedInfo->DnsServerList.IpAddress.String);
        pAddrStr = pFixedInfo->DnsServerList.Next;
        while(pAddrStr)
        {
            printf("%51s\n", pAddrStr->IpAddress.String);
            pAddrStr = pAddrStr->Next;
        }

        printf("\tNode Type . . . . . . . . . : ");
        switch (pFixedInfo->NodeType)
        {
            case 1:
                printf("%s\n", "Broadcast");
                break;
            case 2:
                printf("%s\n", "Peer to peer");
                break;
            case 4:
                printf("%s\n", "Mixed");
                break;
            case 8:
                printf("%s\n", "Hybrid");
                break;
            default:
                printf("\n");
        }

        printf("\tNetBIOS Scope ID. . . . . . : %s\n", pFixedInfo->ScopeId);
        printf("\tIP Routing Enabled. . . . . : %s\n", (pFixedInfo->EnableRouting ? "yes" : "no"));
        printf("\tWINS Proxy Enabled. . . . . : %s\n", (pFixedInfo->EnableProxy ? "yes" : "no"));
        printf("\tNetBIOS Resolution Uses DNS : %s\n", (pFixedInfo->EnableDns ? "yes" : "no"));
    } else
    {
        printf("GetNetworkParams failed with error %d\n", Err);
        return 0;
    }

    //
    // Enumerate all of the adapter specific information using the IP_ADAPTER_INFO structure.
    // Note:  IP_ADAPTER_INFO contains a linked list of adapter entries.
    //
    AdapterInfoSize = 0;
    if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
    {
        if (Err != ERROR_BUFFER_OVERFLOW)
        {
            printf("GetAdaptersInfo sizing failed with error %d\n", Err);
            return 0;
        }
    }

    // Allocate memory from sizing information
    if ((pAdapterInfo = (PIP_ADAPTER_INFO) GlobalAlloc(GPTR, AdapterInfoSize)) == NULL)
    {
        printf("Memory allocation error\n");
        return 0;
    }

    // Get actual adapter information
    if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
    {
        printf("GetAdaptersInfo failed with error %d\n", Err);
        return 0;
    }

    pAdapt = pAdapterInfo;

    while (pAdapt)
    {
        switch (pAdapt->Type)
        {
            case MIB_IF_TYPE_ETHERNET:
                printf("\nEthernet adapter ");
                break;
            case MIB_IF_TYPE_TOKENRING:
                printf("\nToken Ring adapter ");
                break;
            case MIB_IF_TYPE_FDDI:
                printf("\nFDDI adapter ");
                break;
            case MIB_IF_TYPE_PPP:
                printf("\nPPP adapter ");
                break;
            case MIB_IF_TYPE_LOOPBACK:
                printf("\nLoopback adapter ");
                break;
            case MIB_IF_TYPE_SLIP:
                printf("\nSlip adapter ");
                break;
            case MIB_IF_TYPE_OTHER:
            default:
                printf("\nOther adapter ");
        }
        printf("%s:\n\n", pAdapt->AdapterName);

        printf("\tDescription . . . . . . . . : %s\n", pAdapt->Description); 

        printf("\tPhysical Address. . . . . . : ");
	 
        for (i=0; i<pAdapt->AddressLength; i++)
        {
            if (i == (pAdapt->AddressLength - 1))
                printf("%.2X\n",(int)pAdapt->Address[i]);
            else
                printf("%.2X-",(int)pAdapt->Address[i]);
        }        

        printf("\tDHCP Enabled. . . . . . . . : %s\n", (pAdapt->DhcpEnabled ? "yes" : "no"));

        pAddrStr = &(pAdapt->IpAddressList);
        while(pAddrStr)
        {
            printf("\tIP Address. . . . . . . . . : %s\n", pAddrStr->IpAddress.String);
            printf("\tSubnet Mask . . . . . . . . : %s\n", pAddrStr->IpMask.String);
            pAddrStr = pAddrStr->Next;
        }

        printf("\tDefault Gateway . . . . . . : %s\n", pAdapt->GatewayList.IpAddress.String);
        pAddrStr = pAdapt->GatewayList.Next;
        while(pAddrStr)
        {
            printf("%51s\n", pAddrStr->IpAddress.String);
            pAddrStr = pAddrStr->Next;
        }

        printf("\tDHCP Server . . . . . . . . : %s\n", pAdapt->DhcpServer.IpAddress.String);
        printf("\tPrimary WINS Server . . . . : %s\n", pAdapt->PrimaryWinsServer.IpAddress.String);
        printf("\tSecondary WINS Server . . . : %s\n", pAdapt->SecondaryWinsServer.IpAddress.String);

        
        if (error)
        {
            printf("Invalid Argument to _localtime32_s.");
        } else {
            // Convert to an ASCII representation 
            
            if (error)
            {
                printf("Invalid Argument to asctime_s.");
            } else {  
                printf( "\tLease Obtained. . . . . . . : %s", buffer);
            }
        }

        if (error)
        {
            printf("Invalid Argument to _localtime32_s.");
        } else {
            if (error)
            {
                printf("Invalid Argument to asctime_s.");
            } else { 
                printf( "\tLease Expires . . . . . . . : %s", buffer);
            }
        }

        pAdapt = pAdapt->Next;
    }
	
	
//	HDC hdc = GetDC(hwnd);
//	RECT rect;
//	GetClientRect(hwnd, &rect);
//	char * text = (char *)line;
//	DrawTextA(hdc, text, strlen(text), &rect, DT_CENTER);
//	ReleaseDC(hwnd,hdc);

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
//	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
//		TranslateMessage(&msg); /* Translate key codes to chars if present */
//		DispatchMessage(&msg); /* Send it to WndProc */
//	}
//	return msg.wParam;
}
