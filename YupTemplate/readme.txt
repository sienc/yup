# How to release a Project Template

1. Export YupTemplate as a Porject Template

2. Add the collowing lines to .vstemplate

<TemplateData>
	<Name>Yup App</Name>
	<Description>YH's Utilities for Prototyping - Simple App</Description>
	<ProjectType>VC</ProjectType>
	<ProjectSubType>Win32</ProjectSubType>
	<SortOrder>1000</SortOrder>
	<CreateNewFolder>true</CreateNewFolder>
	<DefaultName>YupSimple</DefaultName>
	<ProvideDefaultName>true</ProvideDefaultName>
	<LocationField>Enabled</LocationField>
	<EnableLocationBrowseButton>true</EnableLocationBrowseButton>
	<Icon>__TemplateIcon.ico</Icon>
</TemplateData>

<TemplateContent>
	<CustomParameters>
		<CustomParameter Name="$AppClassName$" Value="$safeprojectname$App"/>
	</CustomParameters>
<TemplateContent>

3. Set ReplaceParameters="true" foe the following items
- main.cpp
- TemplateApp.h
- TemplateApp.cpp

4. Change all TemplateApp to $AppClassName$ in the following files
- .vstemplate
- .vcxproj
- .vcxproj.filters
