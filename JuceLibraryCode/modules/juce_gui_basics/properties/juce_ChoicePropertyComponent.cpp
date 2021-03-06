/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

class ChoicePropertyComponent::RemapperValueSource    : public Value::ValueSource,
                                                        public ValueListener
{
public:
    RemapperValueSource (const Value& sourceValue_, const Array<var>& mappings_)
       : sourceValue (sourceValue_),
         mappings (mappings_)
    {
        sourceValue.addListener (this);
    }

    ~RemapperValueSource() {}

    var getValue() const
    {
        return mappings.indexOf (sourceValue.getValue()) + 1;
    }

    void setValue (const var& newValue)
    {
        const var remappedVal (mappings [(int) newValue - 1]);

        if (remappedVal != sourceValue)
            sourceValue = remappedVal;
    }

    void valueChanged (Value&)
    {
        sendChangeMessage (true);
    }

protected:
    //==============================================================================
    Value sourceValue;
    Array<var> mappings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RemapperValueSource);
};


//==============================================================================
ChoicePropertyComponent::ChoicePropertyComponent (const String& name)
    : PropertyComponent (name),
      isCustomClass (true)
{
}

ChoicePropertyComponent::ChoicePropertyComponent (const Value& valueToControl,
                                                  const String& name,
                                                  const StringArray& choices_,
                                                  const Array <var>& correspondingValues)
    : PropertyComponent (name),
      choices (choices_),
      isCustomClass (false)
{
    // The array of corresponding values must contain one value for each of the items in
    // the choices array!
    jassert (correspondingValues.size() == choices.size());

    createComboBox();

    comboBox.getSelectedIdAsValue().referTo (Value (new RemapperValueSource (valueToControl, correspondingValues)));
}

ChoicePropertyComponent::~ChoicePropertyComponent()
{
}

//==============================================================================
void ChoicePropertyComponent::createComboBox()
{
    addAndMakeVisible (&comboBox);

    for (int i = 0; i < choices.size(); ++i)
    {
        if (choices[i].isNotEmpty())
            comboBox.addItem (choices[i], i + 1);
        else
            comboBox.addSeparator();
    }

    comboBox.setEditableText (false);
}

void ChoicePropertyComponent::setIndex (const int /*newIndex*/)
{
    jassertfalse; // you need to override this method in your subclass!
}

int ChoicePropertyComponent::getIndex() const
{
    jassertfalse; // you need to override this method in your subclass!
    return -1;
}

const StringArray& ChoicePropertyComponent::getChoices() const
{
    return choices;
}

//==============================================================================
void ChoicePropertyComponent::refresh()
{
    if (isCustomClass)
    {
        if (! comboBox.isVisible())
        {
            createComboBox();
            comboBox.addListener (this);
        }

        comboBox.setSelectedId (getIndex() + 1, true);
    }
}

void ChoicePropertyComponent::comboBoxChanged (ComboBox*)
{
    if (isCustomClass)
    {
        const int newIndex = comboBox.getSelectedId() - 1;

        if (newIndex != getIndex())
            setIndex (newIndex);
    }
}
