using Godot;
using System;
using System.Collections.Generic;

public class Player : RigidBody2D
{
    [Export]
    /** Path to this object's camera node */
    private NodePath camPath;

    /** Camera that follows the player */
    private Camera2D _cam;
    
    //[Export]
    /** Maximum velocity from explosions */
    //private float maxVelocity = 5000;

    [Export]
    /** Bomb the player can instance */
    private PackedScene bomb;

    [Export]
    /** Explosion the player can instance */
    private PackedScene explosion;

    /** Bomb currently being held by the player, if any */
    private PlayerBomb _currBomb;

    /** Position object the bomb will be locked to while the player is holding it */
    private Position2D _bombPos;

    /** The bombs this player has fired */
    private List<PlayerBomb> _firedBombs = new List<PlayerBomb>();

    [Export]
    /** Color of the player's right click explosion */
    private Color quickBombColor;

    /** Position of the player on the previous frame */
    private Vector2 _lastPosition = Vector2.Zero;

    public float Health { get; private set; }
    
    [Export]
    private float maxHealth = 50;

    [Signal]
    public delegate void Damaged();

    public float Ammo { get; private set; }

    [Export]
    private float maxAmmo = 500;

    [Export]
    private float ammoRegen = 25;

    [Export]
    private float bombCost = 15;

    [Export]
    private float quickBlastCost = 200;

    [Signal]
    public delegate void AmmoUsed();

    private CollisionShape2D _collider;

    [Export]
    private float rotationRate = 30;

    [Signal]
    public delegate void PlayerDead();

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        _cam = GetNode<Camera2D>(camPath);
        _collider = GetNode<CollisionShape2D>("PlayerCollider");
        _bombPos = _collider.GetNode<Position2D>("BombPosition");
        

        Health = maxHealth;
        Ammo = maxAmmo;
    }

    // Called every frame. 'delta' is the elapsed time since the previous frame.
    public override void _Process(float delta)
    {
        _collider.Rotation = Mathf.LerpAngle(_collider.Rotation, Vector2.Up.AngleTo(GetGlobalMousePosition() - GlobalPosition), rotationRate * delta);/*Vector2.Up.AngleTo(GetGlobalMousePosition() - Position);*/

        _cam.Position = Position;

        // Determine how ammo should be used based on current ammo count and whether a bomb is being charged.
        //If there is no charging bomb, regenerate ammo
        if (_currBomb == null && Ammo < maxAmmo)
        {
            Ammo = Mathf.Clamp(Ammo + (ammoRegen * delta), 0, maxAmmo);
            EmitSignal(nameof(AmmoUsed));
        }

        // If There is a bomb charging, subtract ammo. Stop charging if there is no ammo remaining
        else if (_currBomb != null && !_currBomb.IsCharged && Ammo > 0)
        {
            Ammo -= bombCost * delta;
            if (Ammo <= 0)
            {
                Ammo = 0;
                _currBomb.StopCharging();
            }
            EmitSignal(nameof(AmmoUsed));
        }
        
        /*if(LinearVelocity.Length() > maxVelocity)
        {
            LinearVelocity = LinearVelocity.Clamped(maxVelocity);
        }*/
    }

    public override void _PhysicsProcess(float delta)
    {
        base._PhysicsProcess(delta);

        _lastPosition = Position;
    }

    public override void _Input(InputEvent @event)
    {
        if (@event is InputEventMouseButton mouseButton)
        {
            switch ((ButtonList)mouseButton.ButtonIndex)
            {
                case ButtonList.Left:
                    // If the left mouse button is pressed, create a new bomb and bind it to the ship's bomb charging point
                    if (mouseButton.IsPressed())
                    {
                        _currBomb = bomb.Instance<PlayerBomb>();
                        _currBomb.LockPosition(_bombPos);
                        GetTree().Root.CallDeferred("add_child", _currBomb);
                    }
                    // If the left mouse button is released and there is a bomb held by the ship, detonate it and propel the ship forward
                    else if (_currBomb != null)
                    {
                        PropelShip(_currBomb.explosionForce * (Position - GetGlobalMousePosition()).Normalized());
                        _currBomb.Detonate();
                        _currBomb = null;
                    }
                    break;

                case ButtonList.Right:
                    // If the right mouse button is pressed, create a "quick explosion" and propel the ship
                    if (mouseButton.IsPressed() && _currBomb == null && Ammo >= quickBlastCost)
                    {
                        Explosion ex = explosion.Instance<Explosion>();
                        ex.GlobalPosition = _bombPos.GlobalPosition;
                        ex.Modulate = quickBombColor;
                        GetTree().Root.CallDeferred("add_child", ex);
                        PropelShip(1000 * (Position - GetGlobalMousePosition()).Normalized());
                        
                        //Subtract ammo
                        Ammo -= quickBlastCost;
                        EmitSignal(nameof(AmmoUsed));
                    }
                    break;

                case ButtonList.Middle:
                    if (mouseButton.IsPressed())
                    {
                        // If the middle mouse button is pressed and there is a bomb held, fire it
                        if (_currBomb != null) 
                        {
                            _currBomb.GlobalPosition = _bombPos.GlobalPosition;
                            _currBomb.Fire(LinearVelocity + (500 * (GetGlobalMousePosition() - GlobalPosition).Normalized()));
                            _firedBombs.Add(_currBomb);
                            _currBomb = null;
                        }
                        // If the middle mouse button is pressed and there is no bomb currently held, but there have been bombs fired, detonate them
                        else if (_firedBombs.Count != 0)
                        {
                            // Check this for memory leaks- should be fine but I'm not positive
                            foreach(PlayerBomb bomb in _firedBombs)
                            {
                                try
                                {
                                    bomb.Detonate();
                                } 
                                catch(ObjectDisposedException) { }
                            }

                            _firedBombs = new List<PlayerBomb>();
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }

    /// <summary>
    /// Propel the ship in a given direction using the given force
    /// </summary>
    /// <param name="force">Force to apply to the ship</param>
    private void PropelShip(Vector2 force)
    {
        ApplyCentralImpulse(force);
    }

    /// <summary>
    /// Determines the ship's behavior when it hits an explosion
    /// </summary>
    /// <param name="exp">The explosion which has hit the player</param>
    public void OnExplosion(Explosion exp)
    {
        PropelShip(exp.Power * (_lastPosition - exp.Position).Normalized());
    }

    public void Damage(float damage)
    {
        Health -= damage;

        EmitSignal(nameof(Damaged));

        if(Health <= 0)
        {
            // DIE!!!!
            EmitSignal(nameof(PlayerDead));
            _currBomb?.QueueFree();
            QueueFree();
        }
    }
}
