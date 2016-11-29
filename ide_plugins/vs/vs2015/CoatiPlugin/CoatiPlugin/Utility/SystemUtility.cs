﻿using System;

namespace CoatiSoftware.CoatiPlugin.Utility
{
    class SystemUtility
    {
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        static extern bool SetForegroundWindow(IntPtr hWnd);
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        static extern IntPtr SetActiveWindow(IntPtr hWnd);

        public static void GetWindowFocus()
        {
            try
            {
                System.Diagnostics.Process process = System.Diagnostics.Process.GetCurrentProcess();
                IntPtr windowHandle = process.MainWindowHandle;

                if (windowHandle != null)
                {
                    SetForegroundWindow(windowHandle);
                    SetActiveWindow(windowHandle);
                }
            }
            catch(Exception e)
            {
                Logging.Logging.LogError("Exception: " + e.Message);
            }
        }
    }
}