# creartivity

Einsicht in den Code

1. um den Code laufen zu lassen müssen Sie openFrameworks(oF) und Visual Studio 2022 installieren
2. der "ofCreativeCoding"-Ordner muss in den "apps"-Ordner (..\of_v0.11.0_vs2017_release_apps) von oF verschoben werden
3. fügen Sie die addons mit dem projectGenerator (..\of_v0.11.0_vs2017_release_projectGenerator-vs\projectGenerator.exe) zur App "ofCreativeCoding" hinzu
	3.1 Wie man addons hinzufügt:
Gehen Sie zum Ordner ..osof_v0.11.0_vs2017_release_addons und vergleichen Sie ihn mit dem Inhalt des USB .... und fügen Sie die nicht vorhandenen addons hinzu, indem Sie den gesamten Ordner verschieben.
	3.2 Hinzufügen der addons zur App mit "projectGenerator":
Sobald der "projectGenerator" geöffnet ist, wählen Sie die entsprechenden Ordner mit "import" aus und fügen Sie die erforderlichen addons hinzu

ofInput(..\of_v0.11.0_vs2017_release\apps\ofCreativeCoding\ofInput) - ofxGui, ofxOsc
ofInteraktion(..\of_v0.11.0_vs2017_release\apps\ofCreativeCoding\ofInteraktion) - ofxGui, ofxNI2, ofxOpenCv, ofxOsc
ofProjectManager(..\of_v0.11.0_vs2017_release\apps\ofCreativeCoding\ofProjectManager) - ofxAppUtils, ofxGui, ofxOsc, ofxUbo

Wenn alle addons enthalten sind, klicken Sie jeweils auf "update".

4. öffnen Sie ofProjectManager.sln(..\of_v0.11.0_vs2017_release\apps\ofCreativeCoding\ofProjectManager)
