# coding: utf-8
module Std

  ##
  # Class: Std::Message
  #
  # Parent: TextBox (core)
  #
  # Display a message to the user the same way a TextBox
  # does, i.e. a black box with white text inside appears
  # and the user has to press the Return key in order to
  # continue.
  #
  # Usage example:
  #
  #     m = Message.new("Lorem ipsum dolor sit amet...")
  #     m.activate
  #
  # You can reuse your message as often as you want. Also
  # note this is a subclass of the core TextBox class,
  # which especially allows you to change the message text
  # using the C<MessageBox#text=> method.
  class Message < TextBox

    ##
    # Method: Std::Message::new
    #
    #   new ( text ) → a_message
    #
    # Create a new Message with the given C<text>, which
    # may be multiline.
    def initialize(text)
      super
      self.text = text
      self.massive_type = :passive
      hide
    end

    ##
    # Method: Std::Message#activate
    #
    #   activate()
    #
    # Show the message to the user. This method ensures
    # the text always appears above Alex.
    def activate
      # Ensure we always appear above the player
      self.start_at(Player.x, Player.y - 50)

      # Actual activation
      super

      # Hide afterwards
      self.massive_type = :passive
      warp(-100, 100)
    end

  end

end
