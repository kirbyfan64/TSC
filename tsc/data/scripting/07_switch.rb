# -*- coding: utf-8 -*-
module Std

  ##
  # Class: Std::Switch
  #
  # A switch that can be activated by jumping onto it. This class can either
  # be used to create entirely new switches or attach switch functionality to
  # an already switch sprite.
  #
  # Usage example (where the sprite with UID 14 is a pow sprite):
  #
  #     Std::Switch.new(UIDS[14]) do
  #       puts "Switch activated!"
  #     end
  #
  # Creating a new switch:
  #
  #     Std::Switch.new(x: 200, y: -200) do
  #       puts "Switch activated!"
  #     end
  class Switch

    # The underlying Sprite instance.
    attr_reader :sprite

    # The color of the pow switch.
    attr_reader :color

    ##
    # Method: Std::Switch::new()
    #
    #   new(){...}       → a_switch
    #   new(sprite){...} → a_switch
    #   new(opts){...}   → a_switch
    #
    # Create a new switch. If no parameters are given,
    # a switch is created at position (0|0).
    #
    # =head4 Parameters
    #
    # =over
    #
    # =item [sprite]
    #
    # Use this Sprite instance as the switch. The sprite should
    # have its image set to F<ground/underground/pow.png>.
    #
    # =item [opts]
    #
    # An option hash with the following keys:
    #
    # =over
    #
    # =item [sprite] Same as the C<sprite> parameter above.
    #
    # =item [x] Move the switch to this X position.
    #
    # =item [y] Move the switch to this Y position.
    #
    # =item [color]
    #
    # Color of the switch. Defaults to C<:blue>. Available colors
    # correspond to those listed in the directory
    # F<ground/underground/switch>.
    #
    # =item [save]
    #
    # This option is C<false> by default. If it is a true value,
    # then this method takes care of storing the switch's state
    # into savegames (by registering an appropriate C<save_load>
    # level event handler). If the switch was activated before
    # saving, it will thus be in an active state when the savegame
    # is loaded again.
    #
    # If this option is set specifically to the symbol C<:callback>,
    # then in addition to the restoral of the switch state itself,
    # the callback associated with this switch will be run if on
    # loading a savegame it is found that the switch was in an
    # activated state. This is only useful for simple callbacks
    # and allows to circumvent the need for a manually defined
    # C<save_load> handler.
    #
    # If this option is enabled, the switch must not be created
    # inside a conditional clause. That is, it can only be used
    # if this method is called on the script's toplevel. This
    # restriction comes from the C<save_load> event; see
    # the documentation for the C<LevelClass> class in the core
    # API's documentation for further information.
    #
    # =back
    #
    # If not sprite is passed, setting C<x> and C<y> is recommended,
    # this will be used as the creation position for the new sprite.
    #
    # =back
    #
    # If a block is given, use that block as the action to execute
    # when the switch gets activated.
    #
    # =head4 Return value
    #
    # The newly created instance.
    def initialize(opts = {}, &block)
      @activated = false
      @callback = block || lambda{puts "Dummy switch."}

      # If a single sprite was given, transform for uniform opts handling
      opts = {:sprite => opts} if opts.kind_of?(Sprite)
      opts[:color] ||= :blue
      opts[:save] ||= false

      @color = opts[:color]
      if opts[:sprite]
        @sprite = opts[:sprite]
        @sprite.start_at(opts[:x], opts[:y]) if opts[:x] && opts[:y]
      else
        opts[:x] ||= 0
        opts[:y] ||= 0
        @sprite = Sprite.new("ground/underground/switch/#{@color}.png")
        @sprite.massive_type = :massive
        @sprite.start_at(opts[:x], opts[:y])
        @sprite.show
      end

      @sprite.on_touch do |other|
        if !@activated && other.player? && other.velocity_y > 0
          Audio.play_sound("switch.ogg")
          @sprite.image = "ground/underground/switch/#{@color}_active.png"
          @activated = true
          @callback.call
          other.reset_on_ground
        end
      end

      # Store the switch's state if requested. This conditional execution
      # of Level.on_save_load is okay as it cannot cause a mismatch
      # between saved data and amount of handlers, as long as the Switch
      # instance itself is created not under a conditional.
      if opts[:save]
        @callback_on_load = opts[:save] == :callback

        Level.on_save_load do |store, is_save|
          if is_save
            store["__std_switch_#{@sprite.uid}"] = @activated
          else
            if store["__std_switch_#{@sprite.uid}"]
              @sprite.image = "ground/underground/switch/#{@color}_active.png"
              @activated = true
              @callback.call if @callback_on_load
            end
          end
        end
      end
    end

    ##
    # Method: Std::Switch#on_activate
    #
    #   on_activate() { ... }
    #
    # Specify the action to execute when the switch is activated.
    def on_activate(&block)
      @callback = block
    end

    ##
    # Method: Std::Switch#activated?
    #
    #   activated?() → bool
    #
    # Has the switch already been activated?
    def activated?
      @activated
    end

  end

end
