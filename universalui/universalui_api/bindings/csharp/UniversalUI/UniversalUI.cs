using System.Runtime.InteropServices;

public class UniversalUI {

    [DllImport("libUniversalUI.dll")]
    public static extern int CreateWindow(string title, int width, int height);
}
