#ifndef GUI_UTIL_HH
#define GUI_UTIL_HH

#include <QSettings>
#include "ErrorXOptions.hh"
#include <QApplication>

namespace gui_util {

// Registers the system fonts for Linux
void registerFont( QApplication const & app );

// Builds an ErrorXOptions object from QSettings
// does not address filename, filetype, species, receptor type
errorx::ErrorXOptionsSP optionsFromSettings( QSettings* const settings);

// Takes whatever is in the clipboard and writes to a file
void clipboardToFile( QString const & fileName );

} // namespace gui_util

#endif // GUI_UTIL_HH