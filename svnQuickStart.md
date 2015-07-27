# svn usage #


## Initial checkout like this ##

Get your username from the 'owners' or 'contributors' list on the Project Home page.
Navigate to the directory where you want your local copy to live.
Then

```
svn checkout https://arte-ephys.googlecode.com/svn/trunk/ arte-ephys --username YourUserName
```

## Edit an existing file ##
Simply edit the file, using whatever editor you like.  Then
```
svn commit -m "Describe the commit." --username YourUserName
```
You will be prompted for a password.  This is not your gmail password!  It's a randomly generated string.  To find your password, go to your profile page on google code, and click the 'settings' tab.  Your password will be there front & center.

## Add a new file to an existing directory ##
Create the file in your fav text editor (emacs).  Then:
```
svn add your_file.cpp
svn commit -m "description of file added" --username YourUserName
```
You may be prompted for a password.  See 'Edit an existing file' about this.

## Add a new directory ##
Navigate to the parent directory of the directory you want to create, then.
```
svn mkdir directory_name
svn add directory_name
svn commit -m "description of the directory" --username YourUsername
```
You may be prompted for a pw.  See above about your randomly generated pw.

## Branches, Merging ##
Read up: [here](http://svnbook.red-bean.com/en/1.5/svn-book.html) :)

## Browsing the source in web browser ##
Under 'source' tab, click the 'browse' subtab.  Most of the source will be under trunk/src/.