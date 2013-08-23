api -> api specs and interface specs that plugins must adhere to. Engine will try to load plugins using the api code base
external -> all 3rd party packages used by various plugins
sources -> all data command plugins reside here. Such as web (Twitter, FB, Wiki, Search Engines), db, local, etc
test_* -> tests the specific package. For example, test_sources has test code for sources package
