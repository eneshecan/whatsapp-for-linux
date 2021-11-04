#include "PhoneNumberDialog.hpp"

namespace wfl::ui
{
    PhoneNumberDialog::PhoneNumberDialog(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
        : Gtk::Dialog{cobject}
        , m_okButton{nullptr}
        , m_phoneNumberEntry{nullptr}
    {
        refBuilder->get_widget("ok_button", m_okButton);

        refBuilder->get_widget("phone_number_entry", m_phoneNumberEntry);
        m_phoneNumberEntry->signal_changed().connect(sigc::mem_fun(*this, &PhoneNumberDialog::onTextChange));

        signal_show().connect(sigc::mem_fun(*this, &PhoneNumberDialog::onShow));
        signal_response().connect(sigc::hide(sigc::mem_fun(*this, &PhoneNumberDialog::hide)));

        onTextChange();
    }

    void PhoneNumberDialog::onShow()
    {
        m_phoneNumberEntry->set_text({});
    }

    void PhoneNumberDialog::onTextChange()
    {
        auto const text = m_phoneNumberEntry->get_text();

        m_okButton->set_sensitive(!text.empty() && std::all_of(text.begin(), text.end(), [](unsigned char c) { return std::isdigit(c); }));
    }

    Glib::ustring PhoneNumberDialog::getPhoneNumber()
    {
        return m_phoneNumberEntry->get_text();
    }
}
