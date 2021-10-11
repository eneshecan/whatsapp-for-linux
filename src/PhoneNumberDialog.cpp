#include "PhoneNumberDialog.hpp"

PhoneNumberDialog::PhoneNumberDialog(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
    : Gtk::Dialog{cobject}
    , m_okButton{nullptr}
    , m_phoneNumberEntry{nullptr}
{
    refBuilder->get_widget("ok_button", m_okButton);

    refBuilder->get_widget("phone_number_entry", m_phoneNumberEntry);
    m_phoneNumberEntry->property_text().signal_changed().connect(sigc::mem_fun(*this, &PhoneNumberDialog::onTextChange));

    this->signal_show().connect(sigc::mem_fun(*this, &PhoneNumberDialog::onShow));

    this->onTextChange();
}

void PhoneNumberDialog::onShow()
{
    m_phoneNumberEntry->property_text() = "";
}

void PhoneNumberDialog::onTextChange()
{
    auto const text = m_phoneNumberEntry->get_text();

    // Disable the ok button if the text is empty or contains non-digits
    m_okButton->set_sensitive(!text.empty() && std::all_of(text.begin(), text.end(), [](unsigned char c) { return std::isdigit(c); }));
}

Glib::ustring PhoneNumberDialog::getPhoneNumber()
{
    return m_phoneNumberEntry->get_text();
};
