project = simulinkproject;
projectRoot = project.RootFolder;

myCacheFolder = fullfile(projectRoot, 'cache');
myCodeFolder = fullfile(projectRoot, 'build');

Simulink.fileGenControl('set',...
    'CacheFolder', myCacheFolder,...
    'CodeGenFolder', myCodeFolder,...
    'createDir', true);