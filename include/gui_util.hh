#ifndef GUI_UTIL_HH
#define GUI_UTIL_HH

#include <QSettings>
#include "ErrorXOptions.hh"

namespace gui_util {

// Builds an ErrorXOptions object from QSettings
// does not address filename, filetype, species, receptor type
errorx::ErrorXOptionsSP optionsFromSettings( QSettings* const settings);

// Create checksum of a file to compare if two are equal
QByteArray checksum( QString const & fileName );

// Takes whatever is in the clipboard and writes to a file
void clipboardToFile( QString const & fileName );

} // namespace gui_util

#endif // GUI_UTIL_HH